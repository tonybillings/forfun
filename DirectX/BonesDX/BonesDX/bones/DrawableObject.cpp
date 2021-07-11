#include "DrawableObject.h"

DrawableObject::DrawableObject(const String& name)
	: Object(name), 
	_isVisible(true), 
	_isDefaultLightingEnabled(false), 
	_lightingLayersBitMask(1)
{
	AddComponent(TransformComponent::New(this));
	AddComponent(ModelComponent::New(this));
	AddComponent(ShaderComponent::New(this));

	_transform = &GetComponent<TransformComponent>();
	_model = &GetComponent<ModelComponent>();
	_shader = &GetComponent<ShaderComponent>();
}

void DrawableObject::Tick(const AppState& state)
{
	for (auto&& component : _components)
	{
		if (component->IsEnabled())
			component->Tick(state);
	}

	if (_isDefaultLightingEnabled)
		UpdateDefaultLighting(state);

	if (_isVisible)
		Draw(state);
}

void DrawableObject::EnableDefaultLighting()
{
	_model->SetVertexShaderResource("vs_default_lighting.cso");

	_shader->SetVertexShaderResource("vs_default_lighting.cso");
	_shader->SetPixelShaderResource("ps_default_lighting.cso");

	_isDefaultLightingEnabled = true;
}

void DrawableObject::DisableDefaultLighting()
{
	_isDefaultLightingEnabled = false;
}

void DrawableObject::UpdateDefaultLighting(const AppState& state)
{
	auto camera = CameraComponent::FindActiveCamera();
	if (camera == nullptr) return;

	auto& worldMatrix = _transform->GetWorldMatrix();
	auto& viewProjMatrix = camera->GetViewProjectionMatrix();

	CBTransform vsCB0;
	MStore(&vsCB0.WVP, MMul(worldMatrix, viewProjMatrix));
	MStore(&vsCB0.World, worldMatrix);
	MStore(&vsCB0.WorldInverse, MTranspose(MInv(worldMatrix)));
	_shader->VSSetConstantBufferData(0, &vsCB0, sizeof(vsCB0));

	auto&& material = _model->GetMaterial();
	_shader->PSSetConstantBufferData(0, material->GetData(), material->GetByteWidth());

	auto&& lightComponents = Component::Find<LightComponent>();
	List<CBLightProperties> lights;
	
	for (auto lc : lightComponents)
	{
		if (lights.size() >= 8) break;

		if (lc->IsEnabled())
		{
			auto& transform = lc->GetOwner()->GetComponent<TransformComponent>();
			auto distance = abs(VGetX(VSub(_transform->GetWorldPosition(), transform.GetWorldPosition())));
			
			if (distance <= lc->GetActiveRadius())
			{
				if ((lc->GetActiveLayers() & (uint32_t)(_lightingLayersBitMask)) > 0)
					lights.push_back(lc->Properties);
			}
		}
	}

	CBBasicLighting psCB1;
	VStore(&psCB1.CameraPosition, camera->GetPosition());
	psCB1.GlobalAmbient = LightComponent::GetGlobalAmbient();

	UINT lightIndex = 0;
	for (auto&& light : lights)
		psCB1.Lights[lightIndex++] = light;

	_shader->PSSetConstantBufferData(1, &psCB1, sizeof(psCB1));
}

void DrawableObject::Draw(const AppState& state)
{
	auto ctx = state.Graphics->GetDeviceContext();

	for (auto bindable : _bindables)
	{
		if (!bindable->Bind(ctx))
			return;
	}

	ctx->Draw(_model->GetModel().GetVertexCount(), 0u);
}

Component* DrawableObject::AddComponent(SharedPtr<Component> component)
{
	if (!component) return nullptr;

	if ((*component).GetOwner() == nullptr)
		(*component).SetOwner(this);

	auto bindable = dynamic_cast<IBindable*>(component.get());

	if (bindable != nullptr)
		_bindables.push_back(bindable);

	_components.push_back(std::move(component));

	return component.get();
}