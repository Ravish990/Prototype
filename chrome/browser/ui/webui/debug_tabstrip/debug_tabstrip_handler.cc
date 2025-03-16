#include "chrome/browser/ui/webui/debug_tabstrip/debug_tabstrip_handler.h"

#include "base/json/json_writer.h"
#include "base/values.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "content/public/browser/web_ui.h"

DebugTabstripHandler::DebugTabstripHandler() = default;
DebugTabstripHandler::~DebugTabstripHandler() {
  if (tab_strip_model_)
    tab_strip_model_->RemoveObserver(this);
}

void DebugTabstripHandler::Bind(
    mojo::PendingReceiver<debug_tabstrip::mojom::DebugTabstripHandler> receiver) {
  receiver_.Bind(std::move(receiver));
}

void DebugTabstripHandler::Initialize(InitializeCallback callback) {
  Browser* browser = chrome::FindLastActiveWithProfile(
      Profile::FromWebUI(web_ui()));
  if (!browser) {
    std::move(callback).Run(false);
    return;
  }

  tab_strip_model_ = browser->tab_strip_model();
  std::move(callback).Run(true);
  tab_strip_model_->AddObserver(this);

  // Send initial state
  SendTabStripState();
  SendGroupState();
  SendSelectionState();
  SendSessionState();
}

void DebugTabstripHandler::OnTabStripModelChanged(
    TabStripModel* tab_strip_model,
    const TabStripModelChange& change,
    const TabStripSelectionChange& selection) {
  SendTabStripState();
  SendSelectionState();
}

void DebugTabstripHandler::TabGroupChanged(
    const tab_groups::TabGroupId& group) {
  SendGroupState();
}

void DebugTabstripHandler::GetTabStripState(GetTabStripStateCallback callback) {
  if (!tab_strip_model_) {
    std::move(callback).Run(base::Value::Dict());
    return;
  }

  base::Value::Dict state = DebugTabstripVisualizer::BuildTabStripState(tab_strip_model_);
  std::move(callback).Run(std::move(state));
}

void DebugTabstripHandler::GetGroupState(GetGroupStateCallback callback) {
  if (!tab_strip_model_) {
    std::move(callback).Run(base::Value::Dict());
    return;
  }

  base::Value::Dict state = DebugTabstripVisualizer::BuildGroupState(tab_strip_model_);
  std::move(callback).Run(std::move(state));
}

void DebugTabstripHandler::GetSelectionState(GetSelectionStateCallback callback) {
  if (!tab_strip_model_) {
    std::move(callback).Run(base::Value::Dict());
    return;
  }

  base::Value::Dict state = DebugTabstripVisualizer::BuildSelectionState(tab_strip_model_);
  std::move(callback).Run(std::move(state));
}

void DebugTabstripHandler::GetSessionState(GetSessionStateCallback callback) {
  if (!tab_strip_model_) {
    std::move(callback).Run(base::Value::Dict());
    return;
  }

  base::Value::Dict state = DebugTabstripVisualizer::BuildSessionState(tab_strip_model_);
  std::move(callback).Run(std::move(state));
}