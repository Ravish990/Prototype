#ifndef CHROME_BROWSER_UI_WEBUI_DEBUG_TABSTRIP_DEBUG_TABSTRIP_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_DEBUG_TABSTRIP_DEBUG_TABSTRIP_HANDLER_H_

#include "base/memory/weak_ptr.h"
#include "chrome/browser/ui/webui/debug_tabstrip/mojom/debug_tabstrip.mojom.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/remote.h"

class TabStripModel;
class Profile;

namespace base {
class SequencedTaskRunner;
class RepeatingTimer;
}

// Handles communication between WebUI page and browser for the debug tabstrip.
class DebugTabstripHandler : public debug_tabstrip::mojom::DebugTabstripHandler {
 public:
  DebugTabstripHandler();
  ~DebugTabstripHandler() override;

  DebugTabstripHandler(const DebugTabstripHandler&) = delete;
  DebugTabstripHandler& operator=(const DebugTabstripHandler&) = delete;

  // Binds the mojo interface receiver.
  void Bind(mojo::PendingReceiver<debug_tabstrip::mojom::DebugTabstripHandler> receiver);

  // debug_tabstrip::mojom::DebugTabstripHandler:
  void GetTabStripState(GetTabStripStateCallback callback) override;

 private:
  // Starts the auto-refresh timer
  void StartAutoRefresh();
  
  // Stops the auto-refresh timer
  void StopAutoRefresh();
  
  // Timer callback for auto-refresh
  void OnRefreshTimer();
  
  // Helper method to fetch and send tab strip state
  void FetchAndSendTabStripState(
      mojo::PendingRemote<debug_tabstrip::mojom::DebugTabstripHandlerClient> client);
      
  // Helper method that fetches tab strip state asynchronously on a background thread
  void FetchTabStripStateAsync(
      Profile* profile,
      scoped_refptr<base::SequencedTaskRunner> ui_task_runner,
      mojo::PendingRemote<debug_tabstrip::mojom::DebugTabstripHandlerClient> client);

  mojo::Receiver<debug_tabstrip::mojom::DebugTabstripHandler> receiver_{this};
  TabStripModel* tab_strip_model_ = nullptr;
  content::WebUI* web_ui_ = nullptr;
  
  // Timer for auto-refresh
  std::unique_ptr<base::RepeatingTimer> refresh_timer_;
  
  // Callback stored for auto-refresh
  mojo::PendingRemote<debug_tabstrip::mojom::DebugTabstripHandlerClient> auto_refresh_callback_;
  
  // For generating weak pointers to this instance
  base::WeakPtrFactory<DebugTabstripHandler> weak_factory_{this};
};

#endif  // CHROME_BROWSER_UI_WEBUI_DEBUG_TABSTRIP_DEBUG_TABSTRIP_HANDLER_H_