#ifndef CHROME_BROWSER_UI_WEBUI_DEBUG_TABSTRIP_DEBUG_TABSTRIP_VISUALIZER_H_
#define CHROME_BROWSER_UI_WEBUI_DEBUG_TABSTRIP_DEBUG_TABSTRIP_VISUALIZER_H_

#include "base/values.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "components/sessions/core/session_id.h"
#include "components/tab_groups/tab_group_id.h"
#include "components/sessions/core/session_service.h"

// Helper class to build visualization data structures for the debug WebUI
class DebugTabstripVisualizer {
 public:
  // Creates tab strip state visualization data
  static base::Value::Dict BuildTabStripState(TabStripModel* model);

  // Creates tab group visualization data
  static base::Value::Dict BuildGroupState(TabStripModel* model);

  // Creates selection state visualization data
  static base::Value::Dict BuildSelectionState(TabStripModel* model);

  // Creates session state visualization data
  static base::Value::Dict BuildSessionState(TabStripModel* model);

 private:
  // Helper methods to build individual components of the visualization
  static base::Value::Dict BuildTabData(TabStripModel* model, int index);
  static base::Value::Dict BuildGroupData(TabStripModel* model,
                                        const tab_groups::TabGroupId& group_id);
  static base::Value::Dict BuildSelectionData(TabStripModel* model,
                                            int index);
  static base::Value::Dict BuildSessionData(TabStripModel* model,
                                          const SessionID& session_id);
};

#endif  // CHROME_BROWSER_UI_WEBUI_DEBUG_TABSTRIP_DEBUG_TABSTRIP_VISUALIZER_H_