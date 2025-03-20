#include "chrome/browser/ui/webui/debug_tabstrip/debug_tabstrip_visualizer.h"

#include "base/strings/stringprintf.h"
#include "chrome/browser/ui/tabs/tab_group_model.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "components/tab_groups/tab_group_visual_data.h"

base::Value::Dict DebugTabstripVisualizer::BuildTabStripState(
    TabStripModel* model) {
  base::Value::Dict state;
  base::Value::List tabs;
  tabs.reserve(model->count());

  // Batch process tabs for better performance
  std::vector<content::WebContents*> contents_cache;
  contents_cache.reserve(model->count());
  
  for (int i = 0; i < model->count(); ++i) {
    contents_cache.push_back(model->GetWebContentsAt(i));
  }

  for (int i = 0; i < model->count(); ++i) {
    tabs.Append(BuildTabData(model, i, contents_cache[i]));
  }

  state.Set("total_tabs", model->count());
  state.Set("active_index", model->active_index());
  state.Set("tabs", std::move(tabs));
  
  return state;
}

base::Value::Dict DebugTabstripVisualizer::BuildGroupState(TabStripModel* model) {
  base::Value::Dict state;
  base::Value::List groups;
  
  // Pre-fetch all group IDs for better performance
  std::vector<tab_groups::TabGroupId> group_ids;
  for (int i = 0; i < model->count(); ++i) {
    auto group_id = model->GetTabGroupForTab(i);
    if (group_id.has_value() && 
        std::find(group_ids.begin(), group_ids.end(), group_id) == group_ids.end()) {
      group_ids.push_back(group_id);
    }
  }
  
  groups.reserve(group_ids.size());
  for (const auto& group_id : group_ids) {
    if (group_id.has_value()) {
      groups.Append(BuildGroupData(model, group_id));
    }
  }
  
  state.Set("groups", std::move(groups));
  return state;
  
  state.Set("total_groups", static_cast<int>(groups.size()));
  state.Set("groups", std::move(groups));
  
  return state;
}

base::Value::Dict DebugTabstripVisualizer::BuildSelectionState(
    TabStripModel* model) {
  base::Value::Dict state;
  base::Value::List selected;
  selected.reserve(model->count());

  // Cache selected indices for better performance
  for (int i = 0; i < model->count(); ++i) {
    if (model->IsTabSelected(i)) {
      selected.Append(i);
    }
  }

  state.Set("selected_indices", std::move(selected));
  base::Value::List selection;

  for (int i = 0; i < model->count(); ++i) {
    if (model->IsTabSelected(i)) {
      selection.Append(BuildSelectionData(model, i));
    }
  }

  state.Set("selected_count", static_cast<int>(selection.size()));
  state.Set("selection", std::move(selection));

  return state;
}

base::Value::Dict DebugTabstripVisualizer::BuildSessionState(
    TabStripModel* model) {
  base::Value::Dict state;
  
  // Cache WebContents for better performance
  std::vector<content::WebContents*> contents_cache;
  contents_cache.reserve(model->count());
  for (int i = 0; i < model->count(); ++i) {
    contents_cache.push_back(model->GetWebContentsAt(i));
  }
  
  base::Value::List sessions;
  sessions.reserve(model->count());

  for (int i = 0; i < model->count(); ++i) {
    sessions.Append(BuildSessionData(model, 
        contents_cache[i]->GetSessionID()));
  }

  state.Set("session_count", static_cast<int>(sessions.size()));
  state.Set("sessions", std::move(sessions));

  return state;
}

base::Value::Dict DebugTabstripVisualizer::BuildTabData(
    TabStripModel* model, int index, content::WebContents* contents) {
  base::Value::Dict tab_data;
  
  tab_data.Set("index", index);
  tab_data.Set("title", contents->GetTitle());
  tab_data.Set("url", contents->GetVisibleURL().spec());
  tab_data.Set("is_active", index == model->active_index());
  tab_data.Set("is_pinned", model->IsTabPinned(index));
  tab_data.Set("group_id", model->GetTabGroupForTab(index).ToString());
  
  return tab_data;
}

base::Value::Dict DebugTabstripVisualizer::BuildGroupData(
    TabStripModel* model, const tab_groups::TabGroupId& group_id,
    const std::vector<int>& cached_indices = {}) {
  base::Value::Dict group_data;
  
  // Use cached indices if provided, otherwise compute them
  std::vector<int> tab_indices = cached_indices;
  if (tab_indices.empty()) {
    for (int i = 0; i < model->count(); ++i) {
      if (model->GetTabGroupForTab(i) == group_id) {
        tab_indices.push_back(i);
      }
    }
  }
  
  const tab_groups::TabGroupVisualData* visual_data = 
      model->group_model()->GetTabGroup(group_id)->visual_data();
  
  group_data.Set("id", group_id.ToString());
  group_data.Set("title", visual_data->title());
  group_data.Set("color", static_cast<int>(visual_data->color()));
  
  base::Value::List tabs_in_group;
  for (int i = 0; i < model->count(); ++i) {
    if (model->GetTabGroupForTab(i) == group_id) {
      tabs_in_group.Append(i);
    }
  }
  group_data.Set("tabs", std::move(tabs_in_group));
  
  return group_data;
}

base::Value::Dict DebugTabstripVisualizer::BuildSelectionData(
    TabStripModel* model, int index) {
  base::Value::Dict selection_data;
  content::WebContents* contents = model->GetWebContentsAt(index);
  
  selection_data.Set("index", index);
  selection_data.Set("title", contents->GetTitle());
  selection_data.Set("is_active", index == model->active_index());
  
  return selection_data;
}

base::Value::Dict DebugTabstripVisualizer::BuildSessionData(
    TabStripModel* model, const SessionID& session_id) {
  base::Value::Dict session_data;
  
  session_data.Set("session_id", session_id.id());
  
  // Add session restore data if available
  if (auto* session_service = SessionService::GetService()) {
    if (auto session = session_service->GetTabRestoreData(session_id)) {
      session_data.Set("restore_type", static_cast<int>(session->type));
      session_data.Set("timestamp", session->timestamp.ToInternalValue());
      session_data.Set("current_navigation_index", session->current_navigation_index);
      
      base::Value::List navigations;
      for (const auto& nav : session->navigations) {
        base::Value::Dict nav_data;
        nav_data.Set("url", nav.virtual_url().spec());
        nav_data.Set("title", nav.title());
        nav_data.Set("transition_type", static_cast<int>(nav.transition_type()));
        navigations.Append(std::move(nav_data));
      }
      session_data.Set("navigations", std::move(navigations));
    }
  }
  
  return session_data;
}