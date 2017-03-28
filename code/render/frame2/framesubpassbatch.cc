//------------------------------------------------------------------------------
// framesubpassbatch.cc
// (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "framesubpassbatch.h"
#include "coregraphics/shaderserver.h"
#include "models/visresolver.h"
#include "lighting/lightserver.h"
#include "coregraphics/renderdevice.h"
#include "instancing/instanceserver.h"
#include "materials/materialserver.h"
#include "materials/surface.h"
#include "models/nodes/statenodeinstance.h"
#include "graphics/modelentity.h"

using namespace Graphics;
using namespace CoreGraphics;
using namespace Lighting;
using namespace Materials;
using namespace Instancing;
using namespace Visibility;
using namespace Models;
using namespace Util;

#define NEBULA3_FRAME_LOG_ENABLED   (0)
#if NEBULA3_FRAME_LOG_ENABLED
#define FRAME_LOG(...) n_printf(__VA_ARGS__); n_printf("\n")
#else
#define FRAME_LOG(...)
#endif

namespace Frame2
{

__ImplementClass(Frame2::FrameSubpassBatch, 'FRSB', Frame2::FrameOp);
//------------------------------------------------------------------------------
/**
*/
FrameSubpassBatch::FrameSubpassBatch()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
FrameSubpassBatch::~FrameSubpassBatch()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
FrameSubpassBatch::Run(const IndexT frameIndex)
{
	// now do usual render stuff
	ShaderServer* shaderServer = ShaderServer::Instance();
	VisResolver* visResolver = VisResolver::Instance();
	LightServer* lightServer = LightServer::Instance();
	RenderDevice* renderDevice = RenderDevice::Instance();
	InstanceServer* instanceServer = InstanceServer::Instance();
	MaterialServer* matServer = MaterialServer::Instance();

	// start batch
	renderDevice->BeginBatch(FrameBatchType::Geometry);

	if (matServer->HasMaterialsByBatchGroup(this->batch))
	{
		// get materials matching the batch type
		const Util::Array<Ptr<Material>>& materials = matServer->GetMaterialsByBatchGroup(this->batch);

		IndexT materialIndex;
		for (materialIndex = 0; materialIndex < materials.Size(); materialIndex++)
		{
			// get material
			const Ptr<Material>& material = materials[materialIndex];
			const Util::Array<Material::MaterialPass>& passes = material->GetPassesByCode(this->batch);

			IndexT passIndex;
			for (passIndex = 0; passIndex < passes.Size(); passIndex++)
			{
				// get pass
				const Material::MaterialPass& pass = passes[passIndex];
				const Ptr<Shader>& shader = pass.shader;
				const Util::Array<Ptr<Surface>>& surfaces = material->GetSurfaces();

				IndexT surfaceIndex;
				for (surfaceIndex = 0; surfaceIndex < surfaces.Size(); surfaceIndex++)
				{
					const Ptr<Surface>& surface = surfaces[surfaceIndex];

					// get models based on material, if we can't see any models, just ignore this surface...
					// hmm, would love to do this earlier so we can just skip unused materials
					const Array<Ptr<Model>>& models = visResolver->GetVisibleModels(surface->GetSurfaceCode());
					if (models.IsEmpty()) continue;

					// set the this shader to be the main active shader
					shaderServer->SetActiveShader(shader);

					// reset features, then set the features implemented by the material
					shaderServer->ResetFeatureBits();
					shaderServer->SetFeatureBits(pass.featureMask);

					// apply shared model state (mesh)
					//modelNode->ApplySharedState(frameIndex);

					// apply shader 
					shader->SelectActiveVariation(shaderServer->GetFeatureBits());
					shader->Apply();

					IndexT modelIndex;
					for (modelIndex = 0; modelIndex < models.Size(); modelIndex++)
					{
						FRAME_LOG("      FrameBatch::RenderBatch() model: %s", models[modelIndex]->GetResourceId().Value());

						// for each visible model node of the model...
						const Array<Ptr<ModelNode>>& modelNodes = visResolver->GetVisibleModelNodes(surface->GetSurfaceCode(), models[modelIndex]);
						IndexT modelNodeIndex;
						for (modelNodeIndex = 0; modelNodeIndex < modelNodes.Size(); modelNodeIndex++)
						{
							// render instances
							const Ptr<ModelNode>& modelNode = modelNodes[modelNodeIndex];
							const Array<Ptr<ModelNodeInstance>>& nodeInstances = visResolver->GetVisibleModelNodeInstances(surface->GetSurfaceCode(), modelNode);
							if (nodeInstances.IsEmpty()) continue;

							// apply shared model state (mesh)
							modelNode->ApplySharedState(frameIndex);
							FRAME_LOG("        FrameBatch::RenderBatch() node: %s", modelNode->GetName().Value());

#if NEBULA3_ENABLE_PROFILING
							modelNode->StartTimer();
#endif
							IndexT nodeInstIndex;
							for (nodeInstIndex = 0; nodeInstIndex < nodeInstances.Size(); nodeInstIndex++)
							{
								const Ptr<ModelNodeInstance>& nodeInstance = nodeInstances[nodeInstIndex];
								const Ptr<StateNodeInstance>& stateNode = nodeInstance.downcast<StateNodeInstance>();
								const Ptr<SurfaceInstance>& surfaceInstance = stateNode->GetSurfaceInstance();
#if NEBULA3_ENABLE_PROFILING
								nodeInstance->StartDebugTimer();
#endif  
								// render the node instance
								nodeInstance->ApplyState(frameIndex, pass.index);

								// render single
								nodeInstance->Render();
#if NEBULA3_ENABLE_PROFILING
								modelNode->IncrementDraws();
								nodeInstance->StopDebugTimer();
#endif  
							}

#if NEBULA3_ENABLE_PROFILING
							modelNode->StopTimer();
#endif
						}
					}
				}
			}
		}
	}

	// end batch
	renderDevice->EndBatch();
}

} // namespace Frame2