#pragma once
//------------------------------------------------------------------------------
/**
    @class Widgets::ModelNodeItemHandler
    
    Implements an item handler for ModelNode group boxes
    
    (C) 2012-2015 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"
#include <QObject>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QCheckBox>
#include "ui_modelnodeinfowidget.h"
#include "modelhandler.h"
#include "materials/material.h"
#include "models/nodes/statenodeinstance.h"
#include "resources/managedtexture.h"
#include "materials/managedsurfacematerial.h"

namespace Widgets
{
class ModelNodeHandler : 
	public QObject,
	public Core::RefCounted
{
	Q_OBJECT
	__DeclareClass(ModelNodeHandler);
public:
	/// constructor
	ModelNodeHandler();
	/// destructor
	virtual ~ModelNodeHandler();

	/// sets the ui
	void SetUI(Ui::ModelNodeInfoWidget* ui);
	/// gets the ui
	Ui::ModelNodeInfoWidget* GetUI() const;

	/// set pointer to original item handler
	void SetModelHandler(const Ptr<ModelHandler>& itemHandler);
	/// get pointer to original item handler
	const Ptr<ModelHandler>& GetModelHandler() const;

    /// handle hard reset
    virtual void HardRefresh(const Util::String& resource);
    /// handle soft reset
    virtual void SoftRefresh(const Util::String& resource);

    /// assign surface to state node instance
    void SetSurface(const Util::String& sur);

	/// sets the node name of the handler
	void SetName(const Util::String& name);
	/// gets the node name of the handler
	const Util::String& GetName() const;
	/// sets the node path of the handler
	void SetPath(const Util::String& path);
	/// gets the node path of the handker
	const Util::String& GetPath() const;
	/// set node type
	void SetType(const Util::String& type);
	/// get node
	const Util::String& GetType() const;

	/// constructs the internal structure for the model node item
	virtual void Setup(const Util::String& resource);
    /// discards a model node handler
    void Discard();

private slots:
    /// browse surface material
    void Browse();

protected:

    /// update surface thumbnail icon
    void UpdateSurfaceThumbnail();
	/// clears layout recursively
	void ClearFrame(QLayout* layout);

	bool actionUpdateMode;
	Util::String nodeName;
	Util::String nodePath;
	Util::String nodeType;
	Util::String resource;
	Util::String material;

	QLayout* mainLayout;

	Ui::ModelNodeInfoWidget* ui;
	Ptr<ModelHandler> itemHandler;
    Ptr<Materials::ManagedSurfaceMaterial> managedMaterial;
}; 

//------------------------------------------------------------------------------
/**
*/
inline void
ModelNodeHandler::SetUI(Ui::ModelNodeInfoWidget* ui)
{
	n_assert(ui);
	this->ui = ui;
}

//------------------------------------------------------------------------------
/**
*/
inline Ui::ModelNodeInfoWidget* 
ModelNodeHandler::GetUI() const
{
	return this->ui;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
ModelNodeHandler::SetName( const Util::String& name )
{
	n_assert(name.IsValid());
	this->nodeName = name;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String& 
ModelNodeHandler::GetName() const
{
	return this->nodeName;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
ModelNodeHandler::SetPath( const Util::String& path )
{
	n_assert(path.IsValid());
	this->nodePath = path;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String& 
ModelNodeHandler::GetPath() const
{
	return this->nodePath;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
ModelNodeHandler::SetType( const Util::String& type )
{
	this->nodeType = type;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String& 
ModelNodeHandler::GetType() const
{
	return this->nodeType;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
ModelNodeHandler::SetModelHandler( const Ptr<ModelHandler>& itemHandler )
{
	n_assert(itemHandler.isvalid());
	this->itemHandler = itemHandler;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<ModelHandler>& 
ModelNodeHandler::GetModelHandler() const
{
	return this->itemHandler;
}

} // namespace Widgets
//------------------------------------------------------------------------------