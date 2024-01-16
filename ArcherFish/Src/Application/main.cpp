#include "main.h"

// エントリーポイント
// アプリケーションはこの関数から進行する
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_  HINSTANCE, _In_ LPSTR, _In_ int)
{
	// メモリリークを知らせる
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// COM初期化
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{
		CoUninitialize();

		return 0;
	}

	// mbstowcs_s関数で日本語対応にするために呼ぶ
	setlocale(LC_ALL, "japanese");

	//===================================================================
	// 実行
	//===================================================================
	Application::Instance().Execute();

	// COM解放
	CoUninitialize();

	return 0;
}

// アプリケーション更新の前処理
void Application::PreUpdate()
{
	// 入力状況の更新
	KdInputManager::Instance().Update();
	KdShaderManager::Instance().WorkAmbientController().PreUpdate();

	SceneManager::Instance().PreUpdate();
}

// アプリケーション更新
void Application::Update()
{
	SceneManager::Instance().Update();
}

// アプリケーション更新の後処理
void Application::PostUpdate()
{
	// 3DSoundListnerの行列を更新
	KdAudioManager::Instance().SetListnerMatrix(KdShaderManager::Instance().GetCameraCB().mView.Invert());

	SceneManager::Instance().PostUpdate();
}

// アプリケーション描画の前処理
void Application::PreDraw()
{
	KdDirect3D::Instance().ClearBackBuffer();
	KdShaderManager::Instance().WorkAmbientController().PreDraw();
	KdShaderManager::Instance().m_postProcessShader.PreDraw();

	SceneManager::Instance().PreDraw();
}

// アプリケーション描画
void Application::Draw()
{
	SceneManager::Instance().DrawLit();
}

// アプリケーション描画の後処理
void Application::PostDraw()
{
	// 画面のぼかしや被写界深度処理の実施
	KdShaderManager::Instance().m_postProcessShader.PostEffectProcess();

	SceneManager::Instance().DrawDebug();
}

// 2Dスプライトの描画
void Application::DrawSprite()
{
	SceneManager::Instance().DrawSprite();
}

// アプリケーション初期設定
bool Application::Init(int w, int h)
{
	//===================================================================
	// ウィンドウ作成
	//===================================================================
	if (!m_window.Create(w, h, "Archer Fish", "Window")) 
	{
		MessageBoxA(nullptr, "ウィンドウ作成に失敗", "エラー", MB_OK);
		return false;
	}

	//===================================================================
	// フルスクリーン確認
	//===================================================================
	bool bFullScreen = false;
//	if (MessageBoxA(m_window.GetWndHandle(), "フルスクリーンにしますか？", "確認", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES) { bFullScreen = true; }

	//===================================================================
	// Direct3D初期化
	//===================================================================

	// デバイスのデバッグモードを有効にする
	bool deviceDebugMode = false;
#ifdef _DEBUG
	deviceDebugMode = true;
#endif

	// Direct3D初期化
	std::string errorMsg;
	if (!KdDirect3D::Instance().Init(m_window.GetWndHandle(), w, h, deviceDebugMode, errorMsg)) 
	{
		MessageBoxA(m_window.GetWndHandle(), errorMsg.c_str(), "Direct3D初期化失敗", MB_OK | MB_ICONSTOP);
		return false;
	}

	// フルスクリーン設定
	if (bFullScreen) { KdDirect3D::Instance().WorkSwapChain()->SetFullscreenState(TRUE, 0); }

	//===================================================================
	// imGui
	//===================================================================
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
//	ImGui::StyleColorsClassic();
	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(m_window.GetWndHandle());
	ImGui_ImplDX11_Init(KdDirect3D::Instance().WorkDev(), KdDirect3D::Instance().WorkDevContext());

#include "imGui/ja_glyph_ranges.h"
	ImGuiIO& io = ImGui::GetIO();
	ImFontConfig config;
	config.MergeMode = true;
	io.Fonts->AddFontDefault();
	// 日本語対応
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msgothic.ttc", 13.0f, &config, glyphRangesJapanese);

	//===================================================================
	// シェーダー初期化
	//===================================================================
	KdShaderManager::Instance().Init();
	KdAudioManager::Instance().Init();

	//===================================================================
	// インプット初期化
	//===================================================================
	InputButtonInit();

	//===================================================================
	// サウンドアセット取得
	//===================================================================
	SetSoundInit();

	return true;
}

void Application::InputButtonInit()
{
	// キーボード用コレクター(入力デバイス①)
	KdInputCollector* keyboardCollector = new KdInputCollector();

	// 入力デバイス①をWindowsと命名してマネージャに登録
	KdInputManager::Instance().AddDevice("Windows", keyboardCollector);

	// シーン切替
	/* キーボードキーの入力受付クラスを作成 */
	std::shared_ptr<KdInputButtonForWindows> changeGame;	// Title→Game
	changeGame = std::make_shared<KdInputButtonForWindows>(VK_RETURN);
	/* 入力デバイス①にそれぞれの名前でボタンを登録 */
	keyboardCollector->AddButton("ChangeGame", changeGame);

	std::shared_ptr<KdInputButtonForWindows> changeResult;	// Game→Result
	changeResult = std::make_shared<KdInputButtonForWindows>('O');
	keyboardCollector->AddButton("ChangeResult", changeResult);

	std::shared_ptr<KdInputButtonForWindows> changeTitle;	// Result→Title
	changeTitle = std::make_shared<KdInputButtonForWindows>('P');
	keyboardCollector->AddButton("ChangeTitle", changeTitle);

	// プレイヤーの操作
	std::shared_ptr<KdInputButtonForWindows> moveForward;
	moveForward = std::make_shared<KdInputButtonForWindows>('W');
	keyboardCollector->AddButton("MoveForward", moveForward);

	std::shared_ptr<KdInputButtonForWindows> moveUp;
	moveUp = std::make_shared<KdInputButtonForWindows>('A');
	keyboardCollector->AddButton("MoveUp", moveUp);

	std::shared_ptr<KdInputButtonForWindows> moveDown;
	moveDown = std::make_shared<KdInputButtonForWindows>('D');
	keyboardCollector->AddButton("MoveDown", moveDown);

	std::shared_ptr<KdInputButtonForWindows> shotRay;
	shotRay = std::make_shared<KdInputButtonForWindows>(VK_LBUTTON);
	keyboardCollector->AddButton("ShotRay", shotRay);

	// エネミーの復活
	std::shared_ptr<KdInputButtonForWindows> enemyFlg;
	enemyFlg = std::make_shared<KdInputButtonForWindows>('E');
	keyboardCollector->AddButton("EnemyFlg", enemyFlg);

	// imGui On/Off
	std::shared_ptr<KdInputButtonForWindows> imguiOn;
	imguiOn = std::make_shared<KdInputButtonForWindows>('M');
	keyboardCollector->AddButton("ImGuiOn", imguiOn);

	std::shared_ptr<KdInputButtonForWindows> imguiOff;
	imguiOff = std::make_shared<KdInputButtonForWindows>('N');
	keyboardCollector->AddButton("ImGuiOff", imguiOff);

	// アプリケーション終了
	std::shared_ptr<KdInputButtonForWindows> endButton;
	endButton = std::make_shared<KdInputButtonForWindows>(VK_ESCAPE);
	keyboardCollector->AddButton("EndButton", endButton);
}

void Application::SetSoundInit()
{
	std::initializer_list<std::string_view> soundFiles = {
	"Asset/Sounds/BGM/TitleRipple.wav",
	"Asset/Sounds/SE/GameStart.wav",
	"Asset/Sounds/BGM/GameUnderWater.wav",
	"Asset/Sounds/SE/Predation/Predation.wav",
	"Asset/Sounds/BGM/ResultCoast.wav"
	};

	KdAudioManager::Instance().LoadSoundAssets(soundFiles);
}

// アプリケーション実行
void Application::Execute()
{
	KdCSVData windowData("Asset/Data/WindowSettings.csv");
	const std::vector<std::string>& sizeData = windowData.GetLine(0);

	//===================================================================
	// 初期設定(ウィンドウ作成、Direct3D初期化など)
	//===================================================================
	if (!Application::Instance().Init(atoi(sizeData[0].c_str()), atoi(sizeData[1].c_str()))) { return; }

	//===================================================================
	// ゲームループ
	//===================================================================

	// 時間
	m_fpsController.Init();

	// ループ
	while (true)
	{
		// 処理開始時間Get
		m_fpsController.UpdateStartTime();

		// ゲーム終了指定があるときはループ終了
		if (m_endFlag) { break; }

		//=========================================
		// ウィンドウ関係の処理
		//=========================================

		// ウィンドウのメッセージを処理する
		m_window.ProcessMessage();

		// ウィンドウが破棄されてるならループ終了
		if (!m_window.IsCreated()) { break; }

		if (KdInputManager::Instance().GetButtonState("EndButton")) { End(); }

		// imGui開始
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		//=========================================
		// アプリケーション更新処理
		//=========================================
		PreUpdate();
		Update();
		PostUpdate();

		//=========================================
		// アプリケーション描画処理
		//=========================================
		PreDraw();
		Draw();
		PostDraw();
		DrawSprite();

		// ImGui Demoウィンドウ表示 ※すごく参考になるウィンドウ/imgui_demo.cpp参照
//		ImGui::ShowDemoWindow(nullptr);

		// ImGuiのレンダリング：ここより上にImGuiの描画はする事
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


		// BackBuffer -> 画面表示
		KdDirect3D::Instance().WorkSwapChain()->Present(0, 0);

		//=========================================
		// フレームレート制御
		//=========================================

		m_fpsController.Update();

		// タイトルバーにfps表示
		std::string titleBar = "Archer Fish Fps：" + std::to_string(m_fpsController.m_nowfps);
		SetWindowTextA(m_window.GetWndHandle(), titleBar.c_str());
	}

	//===================================================================
	// アプリケーション解放
	//===================================================================
	Release();
}

// アプリケーション終了
void Application::Release()
{
	KdInputManager::Instance().Release();
	KdShaderManager::Instance().Release();
	KdAudioManager::Instance().Release();

	// imGui解放
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// フルスクリーンを戻す(フルスクリーン前提で記載)
//	KdDirect3D::Instance().WorkSwapChain()->SetFullscreenState(FALSE, 0);

	// Direct3D解放
	KdDirect3D::Instance().Release();

	// ウィンドウ削除
	m_window.Release();
}
