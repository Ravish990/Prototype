void DebugTabstripHandler::FetchAndSendTabStripState(
  mojo::PendingRemote<debug_tabstrip::mojom::DebugTabstripHandlerClient> client) {
// Get a reference to the UI thread task runner for later use
scoped_refptr<base::SequencedTaskRunner> ui_task_runner =
    base::SequencedTaskRunner::GetCurrentDefault();

// Get the profile before going to background thread
Profile* profile = Profile::FromWebUI(web_ui());

// Post task to background thread to fetch tab data asynchronously
base::ThreadPool::PostTask(
    FROM_HERE, {base::MayBlock()},
    base::BindOnce(&DebugTabstripHandler::FetchTabStripStateAsync,
                   weak_factory_.GetWeakPtr(),
                   profile,
                   ui_task_runner,
                   std::move(client)));
}

void DebugTabstripHandler::FetchTabStripStateAsync(
  Profile* profile,
  scoped_refptr<base::SequencedTaskRunner> ui_task_runner,
  mojo::PendingRemote<debug_tabstrip::mojom::DebugTabstripHandlerClient> client) {
// Find the active browser for the current profile
Browser* browser = chrome::FindLastActiveWithProfile(profile);

// Prepare result
base::Value::Dict tab_data;

if (browser) {
  TabStripModel* tab_strip_model = browser->tab_strip_model();
  
  // Get tab group information
  base::Value::Dict groups_dict;
  TabGroupModel* group_model = browser->tab_strip_model()->group_model();
  if (group_model) {
    const std::vector<tab_groups::TabGroupId>& group_ids = group_model->ListTabGroups();
    for (const auto& group_id : group_ids) {
      const TabGroup* group = group_model->GetTabGroup(group_id);
      if (group) {
        base::Value::Dict group_info;
        group_info.Set("id", group_id.ToString());
        group_info.Set("title", group->visual_data().title());
        group_info.Set("color", static_cast<int>(group->visual_data().color()));
        group_info.Set("is_collapsed", group->visual_data().is_collapsed());
        
        // Create a list of tab indices in this group
        base::Value::List tab_indices;
        for (int i = 0; i < tab_strip_model->count(); ++i) {
          absl::optional<tab_groups::TabGroupId> tab_group = 
              tab_strip_model->GetTabGroupForTab(i);
          if (tab_group && tab_group == group_id) {
            tab_indices.Append(i);
          }
        }
        group_info.Set("tab_indices", std::move(tab_indices));
        
        groups_dict.Set(group_id.ToString(), std::move(group_info));
      }
    }
  }
  tab_data.Set("groups", std::move(groups_dict));
  
  // Store active tab index
  tab_data.Set("active_tab_index", tab_strip_model->active_index());
  
  // Create tabs dictionary
  base::Value::Dict tabs_dict;
  
  // Iterate through all tabs in the model
  for (int i = 0; i < tab_strip_model->count(); ++i) {
    const content::WebContents* contents = tab_strip_model->GetWebContentsAt(i);
    if (contents) {
      base::Value::Dict tab_info;
      tab_info.Set("index", i);
      tab_info.Set("title", contents->GetTitle());
      tab_info.Set("url", contents->GetURL().spec());
      tab_info.Set("is_active", tab_strip_model->active_index() == i);
      tab_info.Set("is_pinned", tab_strip_model->IsTabPinned(i));
      tab_info.Set("is_loading", contents->IsLoading());
      
      // Get tab group information for this tab
      absl::optional<tab_groups::TabGroupId> group_id = 
          tab_strip_model->GetTabGroupForTab(i);
      if (group_id) {
        tab_info.Set("has_group", true);
        tab_info.Set("group_id", group_id->ToString());
      } else {
        tab_info.Set("has_group", false);
      }
      
      tabs_dict.Set(base::NumberToString(i), std::move(tab_info));
    }
  }
  tab_data.Set("tabs", std::move(tabs_dict));
}

// Post task back to UI thread to complete the callback
ui_task_runner->PostTask(
    FROM_HERE,
    base::BindOnce(
        [](mojo::PendingRemote<debug_tabstrip::mojom::DebugTabstripHandlerClient> client, 
           base::Value::Dict tab_data) {
          if (client) {
            mojo::Remote<debug_tabstrip::mojom::DebugTabstripHandlerClient> remote_client(
                std::move(client));
            remote_client->OnTabStripStateUpdated(std::move(tab_data));
          }
        },
        std::move(client),
        std::move(tab_data)));
}