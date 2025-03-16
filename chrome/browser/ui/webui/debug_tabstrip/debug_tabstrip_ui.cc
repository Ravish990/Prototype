#include "chrome/browser/ui/webui/debug_tabstrip/debug_tabstrip_ui.h"

#include "chrome/browser/ui/webui/webui_util.h"
#include "chrome/common/url_constants.h"
#include "content/public/browser/web_ui_data_source.h"
#include "ui/base/webui/web_ui_util.h"

DebugTabstripUI::DebugTabstripUI(content::WebUI* web_ui)
    : WebUIController(web_ui) {
  // Create and initialize the data source for this UI.
  content::WebUIDataSource* source =
      content::WebUIDataSource::Create(chrome::kChromeUIDebugTabstripHost);

  // Add required resources
  source->AddResourcePath("debug_tabstrip.js", IDR_DEBUG_TABSTRIP_JS);
  source->SetDefaultResource(IDR_DEBUG_TABSTRIP_HTML);

  content::WebUIDataSource::Add(web_ui->GetWebContents()->GetBrowserContext(),
                               source);
                               
  // Add message handler
  auto handler = std::make_unique<DebugTabstripHandler>();
  web_ui->AddMessageHandler(std::move(handler));
}

DebugTabstripUI::~DebugTabstripUI() = default;