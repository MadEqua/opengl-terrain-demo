#pragma once

#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "EventListener.h"

class Application;

class SceneObject : public EventListener
{
public:
	SceneObject();
	virtual ~SceneObject();

	inline void setMesh(Mesh *mesh) { this->mesh = mesh; }
	inline void setMaterial(Material *material) { this->material = material; }
	inline void setApplication(Application *app) { application = app; }

	virtual void initialize();
	virtual void render(int passID = 0);
	virtual void update(double deltaTime);

	inline Transform& getTransform() { return transform; }
	inline Material& getMaterial() { return *material; }

	inline void setRenderEnabled(bool e) { renderEnabled = e; }
	inline bool isRenderEnabled() { return renderEnabled; }

protected:

	virtual void bindMaterial();

	Transform transform;

	Application *application;
	Material *material;
	Mesh *mesh;

	bool renderEnabled;
};

