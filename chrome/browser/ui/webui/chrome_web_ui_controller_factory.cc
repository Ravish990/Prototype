#include "chrome/browser/ui/webui/chrome_web_ui_controller_factory.h"
#include "chrome/browser/ui/webui/debug_tabstrip/debug_tabstrip_ui.h"
#include "chrome/common/url_constants.h"
#include "content/public/browser/web_ui.h"
#include "url/gurl.h"

std::unique_ptr<content::WebUIController>
ChromeWebUIControllerFactory::CreateWebUIControllerForURL(
    content::WebUI* web_ui,
    const GURL& url) {
      #if BUILDFLAG(ENABLE_DEBUG_TABSTRIP)
  if (web_ui_url.host_piece() == chrome::kChromeUIDebugTabstripHost)
    return NewWebUI<DebugTabstripUI>(web_ui);
#endif

  return nullptr;
}