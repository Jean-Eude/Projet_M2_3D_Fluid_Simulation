#include <Editor.hpp>

Editor::Editor() {

}

void Editor::OnInitUI(GLFWwindow* window) {
    OnReadConfigFile(FilePath::getFilePath("/Assets/EngineParameters/config_engine.flx").c_str());

    m_window_ui = window;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();

    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window_ui, true);
    ImGui_ImplOpenGL3_Init(getGUIVersion().c_str());

    m_io->Fonts->AddFontFromFileTTF(m_iconPath.c_str(), m_fontSize);

    m_icon_range[0] = ICON_MIN_FA;
    m_icon_range[1] = ICON_MAX_FA;
    m_icon_range[2] = 0;
    m_icon_config->MergeMode = true;
    m_icon_config->PixelSnapH = true;
    m_icon_config->GlyphMinAdvanceX = m_iconFontSize;

    // Ordre important ici !
    m_io->Fonts->AddFontFromFileTTF(FilePath::getFilePath("/Assets/EditorParametersAssets/fontawesome-webfont.ttf").c_str(), m_iconFontSize, m_icon_config, m_icon_range);
    m_io->Fonts->AddFontFromFileTTF(FilePath::getFilePath("/Assets/EditorParametersAssets/kenney-icon-font.ttf").c_str(), m_iconFontSize, m_icon_config, m_icon_range);
    m_io->Fonts->AddFontFromFileTTF(FilePath::getFilePath("/Assets/EditorParametersAssets/MaterialSymbolsOutlined[FILL,GRAD,opsz,wght].ttf").c_str(), m_iconFontSize, m_icon_config, m_icon_range);
    m_io->Fonts->AddFontFromFileTTF(FilePath::getFilePath("/Assets/EditorParametersAssets/MaterialSymbolsRounded[FILL,GRAD,opsz,wght].ttf").c_str(), m_iconFontSize, m_icon_config, m_icon_range);
    m_io->Fonts->AddFontFromFileTTF(FilePath::getFilePath("/Assets/EditorParametersAssets/MaterialSymbolsSharp[FILL,GRAD,opsz,wght].ttf").c_str(), m_iconFontSize, m_icon_config, m_icon_range);
    m_io->Fonts->AddFontFromFileTTF(FilePath::getFilePath("/Assets/EditorParametersAssets/lucide.ttf").c_str(), m_iconFontSize, m_icon_config, m_icon_range);
    m_io->Fonts->AddFontFromFileTTF(FilePath::getFilePath("/Assets/EditorParametersAssets/codicon.ttf").c_str(), m_iconFontSize, m_icon_config, m_icon_range);
    m_io->Fonts->AddFontFromFileTTF(FilePath::getFilePath("/Assets/EditorParametersAssets/MaterialIcons-Regular.ttf").c_str(), m_iconFontSize, m_icon_config, m_icon_range);
    m_io->Fonts->AddFontFromFileTTF(FilePath::getFilePath("/Assets/EditorParametersAssets/materialdesignicons-webfont.ttf").c_str(), m_iconFontSize, m_icon_config, m_icon_range);
    m_io->Fonts->Build();

    pushLayers();

    layerStack.InitLayersAndOverlays();
}

void Editor::OnUpdateUI() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if(m_isUIDocked == true) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
        ImGui::DockSpaceOverViewport(dockspace_id, viewport, ImGuiDockNodeFlags_None, nullptr);
    }

    layerStack.RenderLayersAndOverlays();

    ImGui::Render();
}

void Editor::OnRenderUI() {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::OnDestroyUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

std::vector<LayerEditor*> Editor::CreateLayers(LayerStackEditor& stackEditor) {
    return {
        new LogsEditorLayer("Layer_Logs"),
        new ToolsEditorLayer("Layer_Tools"),
        new ViewportEditorLayer("Layer_Viewport"),
        //new ParametersLayerEditor("Layer_Parameters"),
        new ParticlesManager("Layer_Particles"),
    };
}

