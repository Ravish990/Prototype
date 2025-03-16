// Add to existing WebUI bindings
#include "chrome/browser/ui/webui/debug_tabstrip/debug_tabstrip_ui.h"
#include "chrome/common/url_constants.h"

// In ChromeWebUIControllerFactory::CreateWebUIControllerForURL method:
if (url.host_piece() == chrome::kChromeUIDebugTabstripHost)
  return std::make_unique<DebugTabstripUI>(web_ui);