#ifndef CHROME_BROWSER_UI_WEBUI_DEBUG_TABSTRIP_DEBUG_TABSTRIP_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_DEBUG_TABSTRIP_DEBUG_TABSTRIP_HANDLER_H_

#include "base/memory/raw_ptr.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "content/public/browser/web_ui_message_handler.h"
#include "chrome/browser/ui/webui/debug_tabstrip/debug_tabstrip_visualizer.h"

// Handles messages from the JavaScript to the C++ code for the debug tabstrip WebUI.
class DebugTabstripHandler : public content::WebUIMessageHandler,
                           public TabStripModelObserver {
 public:
  DebugTabstripHandler();
  ~DebugTabstripHandler() override;

  DebugTabstripHandler(const DebugTabstripHandler&) = delete;
  DebugTabstripHandler& operator=(const DebugTabstripHandler&) = delete;

  // WebUIMessageHandler implementation.
  void RegisterMessages() override;

  // TabStripModelObserver implementation.
  void OnTabStripModelChanged(
      TabStripModel* tab_strip_model,
      const TabStripModelChange& change,
      const TabStripSelectionChange& selection) override;
  void TabGroupChanged(const tab_groups::TabGroupId& group);

 private:
  // Handler for the 'initialize' message from JavaScript.
  void HandleInitialize(const base::Value::List& args);
  
  // Sends updated tab strip state to the WebUI.
  void SendTabStripState();
  void SendGroupState();
  void SendSelectionState();
  void SendSessionState();

  // The associated TabStripModel that we're debugging.
  raw_ptr<TabStripModel> tab_strip_model_ = nullptr;
};

#endif  // CHROME_BROWSER_UI_WEBUI_DEBUG_TABSTRIP_DEBUG_TABSTRIP_HANDLER_H_