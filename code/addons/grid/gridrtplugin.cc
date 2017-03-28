//------------------------------------------------------------------------------
//  gridrtplugin.cc
//  (C) 2012-2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "gridrtplugin.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/memoryvertexbufferloader.h"
#include "coregraphics/memoryindexbufferloader.h"
#include "coregraphics/renderdevice.h"
#include "coregraphics/transformdevice.h"
#include "coregraphics/displaydevice.h"
#include "coregraphics/config.h"
#include "resources/resourcemanager.h"

using namespace Math;
using namespace CoreGraphics;
using namespace Resources;
namespace Grid
{
__ImplementClass(Grid::GridRTPlugin, 'GRRT', RenderModules::RTPlugin);

//------------------------------------------------------------------------------
/**
*/
GridRTPlugin::GridRTPlugin() :
	visible(true),
	gridSize(1)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
GridRTPlugin::~GridRTPlugin()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
GridRTPlugin::OnRegister()
{
	// create new shader
	this->shader = ShaderServer::Instance()->CreateShaderState("shd:grid", {NEBULAT_DEFAULT_GROUP});
	this->gridSizeVar = this->shader->GetVariableByName("GridSize");
	this->gridTexVar = this->shader->GetVariableByName("GridTex");

	// load texture
	this->tex = ResourceManager::Instance()->CreateManagedResource(Texture::RTTI, "tex:system/grid.dds").downcast<ManagedTexture>();

#define VERTEX_POS 250.0f
	// setup VBO
	Util::Array<VertexComponent> components;
	components.Append(VertexComponent(VertexComponent::Position, 0, VertexComponent::Float2, 0));
	float verts[] = { -VERTEX_POS, -VERTEX_POS, VERTEX_POS, -VERTEX_POS, VERTEX_POS, VERTEX_POS, -VERTEX_POS, VERTEX_POS };
	Ptr<MemoryVertexBufferLoader> vboLoader = MemoryVertexBufferLoader::Create();
	vboLoader->Setup(components, 4, verts, sizeof(verts), VertexBuffer::UsageImmutable, VertexBuffer::AccessNone);
	
	this->vbo = VertexBuffer::Create();
	this->vbo->SetLoader(vboLoader.upcast<ResourceLoader>());
	this->vbo->SetAsyncEnabled(false);
	this->vbo->Load();
	n_assert(this->vbo->IsLoaded());
	this->vbo->SetLoader(NULL);	

	short indices[] = { 0, 1, 2, 2, 3, 0 };
	Ptr<MemoryIndexBufferLoader> iboLoader = MemoryIndexBufferLoader::Create();
	iboLoader->Setup(IndexType::Index16, 6, indices, sizeof(indices));

	this->ibo = IndexBuffer::Create();
	this->ibo->SetLoader(iboLoader.upcast<ResourceLoader>());
	this->ibo->SetAsyncEnabled(false);
	this->ibo->Load();
	n_assert(this->ibo->IsLoaded());
	this->ibo->SetLoader(NULL);	
	
	this->primitive.SetBaseIndex(0);
	this->primitive.SetNumVertices(4);
	this->primitive.SetBaseIndex(0);
	this->primitive.SetNumIndices(6);
}

//------------------------------------------------------------------------------
/**
*/
void
GridRTPlugin::OnUnregister()
{
	ResourceManager::Instance()->DiscardManagedResource(this->tex.upcast<ManagedResource>());
	this->tex = 0;
	this->ibo->Unload();
	this->ibo = 0;
	this->vbo->Unload();
	this->vbo = 0;

	this->gridSizeVar = 0;
	this->gridTexVar = 0;
	this->shader = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
GridRTPlugin::OnRender(const Util::StringAtom& filter)
{
	static const Util::StringAtom identifier = "Unlit";
	if (filter == identifier && this->visible)
	{
		Ptr<RenderDevice> device = RenderDevice::Instance();
		Ptr<TransformDevice> trans = TransformDevice::Instance();

		// start pass
		this->shader->Apply();

		// set variables
		//this->shader->BeginUpdate();
		this->gridSizeVar->SetFloat(this->gridSize);
		this->gridTexVar->SetTexture(this->tex->GetTexture());
//		this->shader->EndUpdate();
		this->shader->Commit();

		device->SetVertexLayout(this->vbo->GetVertexLayout());
		device->SetPrimitiveGroup(this->primitive);
		device->SetPrimitiveTopology(PrimitiveTopology::TriangleList);
		device->SetStreamVertexBuffer(0, this->vbo, 0);
		device->SetIndexBuffer(this->ibo);
		device->Draw();
	}
}

} // namespace Grid
