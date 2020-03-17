#include "text_visual_2d_component.h"

#include "systems/render_system.h"

Component* TextVisual2DComponent::Create(const JSON::json& componentData)
{
	TextVisual2DComponent* tV2DC = new TextVisual2DComponent(
		ResourceLoader::CreateFontResourceFile(
			componentData["fontResource"], 
			componentData["name"]), 
		componentData["text"], 
		{ 
			componentData["color"]["r"], 
			componentData["color"]["g"], 
			componentData["color"]["b"], 
			componentData["color"]["a"] 
		}, 
		(Mode)(int)componentData["mode"]);
	return tV2DC;
}

Component* TextVisual2DComponent::CreateDefault()
{
	return new TextVisual2DComponent(ResourceLoader::CreateFontResourceFile("game/assets/fonts/noto_sans_50_regular.spritefont", "Noto Sans"), "Hello World!", { 1.0f, 1.0f, 1.0f, 1.0f }, Mode::None);
}

TextVisual2DComponent::TextVisual2DComponent(FontResourceFile* font, const String& text, const Color& color, const Mode& mode)
    : m_Font(font)
    , m_Text(text)
    , m_Color(color)
    , m_Mode(mode)
{
}

TextVisual2DComponent::~TextVisual2DComponent()
{
}

void TextVisual2DComponent::render(HierarchyGraph* graph)
{
	static Vector3 position;
	static Quaternion rotation;
	static float rotationAngle;
	static Vector3 scale;

	RenderSystem::GetSingleton()->getTopUIMatrix().Decompose(scale, rotation, position);
	rotationAngle = Vector3((Vector3(0.0f, 0.0f, 1.0f) * rotation)).z;

	m_Font->getFont()->DrawString(
	    RenderingDevice::GetSingleton()->getUIBatch().get(),
	    m_Text.c_str(),
	    position,
	    m_Color,
		rotationAngle, 
		{ 0.0f, 0.0f },
		scale, 
		(DirectX::SpriteEffects)m_Mode);
}

JSON::json TextVisual2DComponent::getJSON() const
{
	JSON::json j;

	j["fontResource"] = m_Font->getPath().string();
	j["name"] = m_Font->getFontName();
	j["text"] = m_Text;

	j["color"]["r"] = m_Color.x;
	j["color"]["g"] = m_Color.y;
	j["color"]["b"] = m_Color.z;
	j["color"]["a"] = m_Color.w;

	j["mode"] = (int)m_Mode;

	return j;
}

#ifdef ROOTEX_EDITOR
#include "imgui.h"
#include "imgui_stdlib.h"
void TextVisual2DComponent::draw()
{
	ImGui::InputText("Text", &m_Text);
	ImGui::ColorEdit4("Color", &m_Color.x);

	static const char* modes[] = {
		"None",
		"FlipX",
		"FlipY",
		"FlipXY"
	};

	static int choice = 0;
	ImGui::Combo("Mode", &choice, modes, IM_ARRAYSIZE(modes));
	m_Mode = (Mode)choice;
}
#endif // ROOTEX_EDITOR
