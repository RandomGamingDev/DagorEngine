#pragma once

#include "av_tree.h"
#include "av_environment.h"
#include "Entity/compositeEditor.h"

#include <sepGui/wndPublic.h>

#include <propPanel2/comWnd/panel_window.h>
#include <propPanel2/comWnd/tool_window.h>

#include <EditorCore/ec_interface.h>
#include <EditorCore/ec_genappwnd.h>

#include <libTools/dagFileRW/textureNameResolver.h>

#include <assets/assetMgr.h>
#include <assets/assetChangeNotify.h>

#include <util/dag_string.h>
#include <util/dag_simpleString.h>

#include <EASTL/unique_ptr.h>

enum
{
  GUI_PLUGIN_TOOLBAR_ID,
};


void init_all_editor_plugins();

class ImpostorGenerator;
struct ImpostorOptions;

class CompositeEditor;
class ColorDialogAppMat;

class AssetViewerApp;
AssetViewerApp &get_app();


class AssetViewerApp : public GenericEditorAppWindow,
                       public IEditorCoreEngine,
                       public ITextureNameResolver,
                       public IDagorAssetChangeNotify,
                       public ControlEventHandler,
                       public ITreeViewEventHandler,
                       public IConsoleCmd,
                       public IWndManagerWindowHandler
{
public:
  AssetViewerApp(IWndManager *manager, const char *open_fname = NULL);
  ~AssetViewerApp();

  static const char *build_version;

  inline void repaint();
  inline PropertyContainerControlBase *getPropPanel() const;
  inline void *getAdditinalPropWindow() const;

  void fillPropPanel();
  void setScriptChangeFlag() { scriptChangeFlag = true; }
  bool getScriptChangeFlag() { return scriptChangeFlag; }
  void fillToolBar();

  virtual void init();

  void renderGrid(bool render) { grid.setVisible(render, 0); }
  bool isGridVisible() { return grid.isVisible(0); }
  const char *getMatParamsPath() { return matParamsPath; }

  // IEditorCoreEngine
  // ==========================================================================
  // query/get interfaces
  virtual void *queryEditorInterfacePtr(unsigned huid);

  virtual void screenshotRender();

  // register/unregister plugins(every plugin should be registered once)
  virtual bool registerPlugin(IGenEditorPlugin *plugin);
  virtual bool unregisterPlugin(IGenEditorPlugin *plugin);

  // plugins management.
  virtual int getPluginCount();
  virtual IGenEditorPlugin *getPlugin(int idx);
  virtual int getPluginIdx(IGenEditorPlugin *plug) const;
  virtual IGenEditorPlugin *curPlugin();

  virtual IGenEditorPluginBase *getPluginBase(int idx);
  virtual IGenEditorPluginBase *curPluginBase();

  virtual void *getInterface(int interface_uid);
  virtual void getInterfaces(int interface_uid, Tab<void *> &interfaces);

  // UI management
  virtual IWndManager *getWndManager() const;
  virtual PropPanel2 *getCustomPanel(int id) const;
  virtual void *addToolbar(hdpi::Px height);
  virtual CToolWindow *createToolbar(ControlEventHandler *eh, void *hwnd) { return NULL; }
  virtual void addPropPanel(int type, hdpi::Px width);
  virtual void removePropPanel(void *hwnd);
  virtual void managePropPanels() {}
  virtual void skipManagePropPanels(bool skip) {}
  virtual CPanelWindow *createPropPanel(ControlEventHandler *eh, void *hwnd) { return NULL; }
  virtual void deleteCustomPanel(PropPanel2 *panel) {}
  virtual CDialogWindow *createDialog(hdpi::Px w, hdpi::Px h, const char *caption) { return NULL; }
  virtual void deleteDialog(CDialogWindow *dlg) {}

  // viewport methods
  virtual void updateViewports();
  virtual void setViewportCacheMode(ViewportCacheMode mode);
  virtual void invalidateViewportCache();
  virtual int getViewportCount();
  virtual IGenViewportWnd *getViewport(int n);
  virtual IGenViewportWnd *getRenderViewport();
  virtual IGenViewportWnd *getCurrentViewport();
  virtual void setViewportZnearZfar(real zn, real zf);

  // ray tracing methods
  virtual IGenViewportWnd *screenToViewport(int &x, int &y);
  virtual bool screenToWorldTrace(int x, int y, Point3 &world, real maxdist = 1000.0, Point3 *out_norm = NULL) { return false; }
  virtual bool clientToWorldTrace(IGenViewportWnd *wnd, int x, int y, Point3 &world, real maxdist = 1000.0, Point3 *out_norm = NULL)
  {
    return false;
  }
  virtual void setupColliderParams(int mode, const BBox3 &area);
  virtual bool traceRay(const Point3 &src, const Point3 &dir, float &dist, Point3 *out_norm = NULL, bool use_zero_plane = true);

  virtual void setColliders(dag::ConstSpan<IDagorEdCustomCollider *> c, unsigned filter_mask) const {};
  virtual void restoreEditorColliders() const {}
  virtual float getMaxTraceDistance() const { return 1500; }

  virtual bool getSelectionBox(BBox3 &box);
  virtual void zoomAndCenter();

  virtual String getScreenshotNameMask(bool cube) const;

  // gizmo methods
  virtual void setGizmo(IGizmoClient *gc, ModeType type);
  virtual void startGizmo(IGenViewportWnd *wnd, int x, int y, bool inside, int buttons, int key_modif);
  virtual ModeType getGizmoModeType(); //{ return MODE_Move; }
  virtual BasisType getGizmoBasisType() { return BASIS_World; }
  virtual CenterType getGizmoCenterType() { return CENTER_Pivot; }
  bool isGizmoOperationStarted() const;

  // brush methods
  virtual void beginBrushPaint() {}
  virtual void renderBrush() {}
  virtual void setBrush(Brush *brush) {}
  virtual void endBrushPaint() {}
  virtual Brush *getBrush() const { return NULL; }
  virtual bool isBrushPainting() const { return false; }

  // spatial cursor handling
  virtual void showUiCursor(bool vis) {}
  virtual void setUiCursorPos(const Point3 &pos, const Point3 *norm = NULL) {}
  virtual void getUiCursorPos(Point3 &pos, Point3 &norm) {}
  virtual void setUiCursorTex(TEXTUREID tex_id) {}
  virtual void setUiCursorProps(float size, bool always_xz) {}

  // internal interface
  virtual void actObjects(real dt);
  virtual void beforeRenderObjects();
  virtual void renderObjects();
  virtual void renderIslDecalObjects() {}
  virtual void renderTransObjects();

  virtual void showSelectWindow(IObjectsList *obj_list, const char *obj_list_owner_name);

  virtual UndoSystem *getUndoSystem() { return undoSystem; }
  virtual CoolConsole &getConsole()
  {
    G_ASSERT(console);
    return *console;
  }
  virtual GridObject &getGrid() { return grid; }

  virtual Point3 snapToGrid(const Point3 &p) const;
  virtual Point3 snapToAngle(const Point3 &p) const;
  virtual Point3 snapToScale(const Point3 &p) const;

  virtual const char *getLibDir() const { return NULL; }
  virtual class LibCache *getLibCachePtr() { return NULL; }
  virtual Tab<struct WspLibData> *getLibData() { return NULL; }
  virtual const EditorWorkspace &getBaseWorkspace() { return GenericEditorAppWindow::getWorkspace(); }

  // ==========================================================================

  // ITextureNameResolver
  virtual bool resolveTextureName(const char *src_name, String &out_str);

  // IDagorAssetChangeNotify
  virtual void onAssetRemoved(int asset_name_id, int asset_type);
  virtual void onAssetChanged(const DagorAsset &asset, int asset_name_id, int asset_type);

  // IWndManagerWindowHandler
  virtual IWndEmbeddedWindow *onWmCreateWindow(void *handle, int type);
  virtual bool onWmDestroyWindow(void *handle);

  // IConsoleCmd
  virtual bool onConsoleCommand(const char *cmd, dag::ConstSpan<const char *> params);
  virtual const char *onConsoleCommandHelp(const char *cmd);

  void drawAssetInformation(IGenViewportWnd *wnd);
  void afterUpToDateCheck(bool changed);
  const DagorAssetMgr &getAssetMgr() const { return assetMgr; }
  bool trackChangesContinuous(int assets_to_check);
  void invalidateAssetIfChanged(DagorAsset &a);
  virtual IWndManager &getWndManager()
  {
    G_ASSERT(mManager);
    return *mManager;
  }

  const DagorAsset *getCurAsset() const { return curAsset; }
  bool reloadAsset(const DagorAsset &asset, int asset_name_id, int asset_type);
  void refillTree();
  void selectAsset(const DagorAsset &asset, bool reset_filter_if_needed);

  CompositeEditor &getCompositeEditor() { return compositeEditor; }
  ImpostorGenerator *getImpostorGenerator() const { return impostorApp.get(); }
  bool canRenderEnvi() const { return !skipRenderEnvi; }
  bool isCompositeEditorShown() const;
  void showCompositeEditor(bool show);

protected:
  virtual bool handleNewProject(bool edit = false) { return false; }
  virtual bool handleOpenProject(bool edit = false) { return false; }
  // virtual bool canCreateNewProject(const char *filename) { return false; }  // not used

  virtual bool createNewProject(const char *filename) { return false; }
  virtual bool loadProject(const char *filename);
  virtual bool saveProject(const char *filename);

  virtual void fillMenu(IMenu *menu);
  virtual void updateMenu(IMenu *menu);

  virtual void getDocTitleText(String &text);
  virtual bool canCloseScene(const char *title);

  int findPlugin(IGenEditorPlugin *p);
  void sortPlugins() {}

  void terminateInterface();
  void switchToPlugin(int id);
  void splitProjectFilename(const char *filename, String &path, String &name);

  // ControlEventHandler
  virtual void onClick(int pcb_id, PropertyContainerControlBase *panel);

  // ITreeViewEventHandler
  virtual void onTvSelectionChange(TreeBaseWindow &tree, TLeafHandle new_sel);
  virtual bool onTvContextMenu(TreeBaseWindow &tree, TLeafHandle under_mouse, IMenu &menu);

  // Menu
  int onMenuItemClick(unsigned id);

  void makeDefaultLayout();
  void fillTree();
  void saveTreeState();

  void applyDiscardAssetTexMode();

private:
  SimpleString matParamsPath;
  IGenEditorPlugin *getTypeSupporter(const DagorAsset *asset) const;

  // from EditorCore
  Tab<IGenEditorPlugin *> plugin;
  int curPluginId;

  DagorAssetMgr assetMgr;

  DagorAsset *curAsset;
  String curAssetPackName;

  int allUpToDateFlags;

  void *hwndTree, *hwndPPanel, *hwndToolbar, *hwndViewPort;
  void *hwndPluginPanel, *hwndPluginToolbar;
  AvTree *mTreeView;
  DataBlock propPanelState;
  CPanelWindow *mPropPanel;
  CToolWindow *mToolPanel;
  CToolWindow *mPluginTool;

  eastl::unique_ptr<ImpostorGenerator> impostorApp;
  eastl::unique_ptr<ColorDialogAppMat> colorPaletteDlg;
  CompositeEditor compositeEditor;

  // int allUpToDateFlags;

  bool blockSave;
  bool fillPropPanelHasBeenCalled = false;
  bool scriptChangeFlag;
  bool autoZoomAndCenter;
  bool discardAssetTexMode;
  bool skipRenderObjects = false;
  bool skipRenderEnvi = false;

  AssetLightData assetLtData, assetDefaultLtData;

  void onClosing();

  void renderGrid();

  void blockModifyRoutine(bool block);

  void showPropWindow(bool is_show);
  void showAdditinalPropWindow(bool is_show);
  void showAdditinalToolWindow(bool is_show);

  bool runShadersListVars(dag::ConstSpan<const char *> params);
  bool runShadersSetVar(dag::ConstSpan<const char *> params);
  bool runShadersReload(dag::ConstSpan<const char *> params);

  void generate_impostors(const ImpostorOptions &options);
  void clear_impostors(const ImpostorOptions &options);

  void createAssetsTree();
  void createToolbar();
};


//=============================================================================
// inine functions
//=============================================================================
inline IGenEditorPlugin *AssetViewerApp::curPlugin() { return curPluginId == -1 ? NULL : plugin[curPluginId]; }


//=============================================================================
inline void AssetViewerApp::repaint()
{
  updateViewports();
  invalidateViewportCache();
}


//=============================================================================
inline PropertyContainerControlBase *AssetViewerApp::getPropPanel() const { return mPropPanel; }


inline void *AssetViewerApp::getAdditinalPropWindow() const { return hwndPluginPanel; }
