#ifndef CHROME_BROWSER_UI_WEBUI_DEBUG_TABSTRIP_DEBUG_TABSTRIP_UI_H_
#define CHROME_BROWSER_UI_WEBUI_DEBUG_TABSTRIP_DEBUG_TABSTRIP_UI_H_

#include "content/public/browser/web_ui_controller.h"
#include "chrome/browser/ui/webui/debug_tabstrip/debug_tabstrip_handler.h"

// WebUI controller for chrome://debug-tabstrip
class DebugTabstripUI : public content::WebUIController {
 public:
  explicit DebugTabstripUI(content::WebUI* web_ui);
  ~DebugTabstripUI() override;

  DebugTabstripUI(const DebugTabstripUI&) = delete;
  DebugTabstripUI& operator=(const DebugTabstripUI&) = delete;
};

#endif  // CHROME_BROWSER_UI_WEBUI_DEBUG_TABSTRIP_DEBUG_TABSTRIP_UI_H_