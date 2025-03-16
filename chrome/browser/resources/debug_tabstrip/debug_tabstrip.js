// Copyright 2025 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/**
 * @fileoverview Debug view for tabstrip state visualization.
 */

'use strict';

/**
 * Updates the tabstrip tree visualization.
 * @param {Object} treeData The tree data structure representing current tabstrip state
 */
function updateTabstripTree(treeData) {
  const treeElement = document.getElementById('tabstrip-tree');
  treeElement.innerHTML = '';

  const totalTabs = document.createElement('div');
  totalTabs.textContent = `Total Tabs: ${treeData.total_tabs}`;
  treeElement.appendChild(totalTabs);

  const activeTab = document.createElement('div');
  activeTab.textContent = `Active Tab Index: ${treeData.active_index}`;
  treeElement.appendChild(activeTab);

  const tabList = document.createElement('ul');
  tabList.className = 'tab-list';
  treeData.tabs.forEach(tab => {
    const tabItem = document.createElement('li');
    tabItem.className = 'tab-item';
    if (tab.is_active) {
      tabItem.classList.add('active');
    }
    if (tab.is_pinned) {
      tabItem.classList.add('pinned');
    }
    
    tabItem.innerHTML = `
      <div class="tab-title">${tab.title}</div>
      <div class="tab-url">${tab.url}</div>
      <div class="tab-meta">
        Index: ${tab.index}
        ${tab.is_pinned ? '📌' : ''}
        ${tab.group_id ? `Group: ${tab.group_id}` : ''}
      </div>
    `;
    
    tabList.appendChild(tabItem);
  });
  
  treeElement.appendChild(tabList);
}

/**
 * Updates the group information display.
 * @param {Object} groupData Data about tab groups
 */
function updateGroupInfo(groupData) {
  const groupElement = document.getElementById('group-info');
  groupElement.innerHTML = '';

  const totalGroups = document.createElement('div');
  totalGroups.textContent = `Total Groups: ${groupData.total_groups}`;
  groupElement.appendChild(totalGroups);

  const groupList = document.createElement('div');
  groupList.className = 'group-list';
  groupData.groups.forEach(group => {
    const groupItem = document.createElement('div');
    groupItem.className = 'group-item';
    
    const colorClass = `group-color-${group.color}`;
    groupItem.classList.add(colorClass);
    
    groupItem.innerHTML = `
      <div class="group-header">
        <span class="group-title">${group.title}</span>
        <span class="group-id">(${group.id})</span>
      </div>
      <div class="group-tabs">
        Tabs in group: ${group.tabs.join(', ')}
      </div>
    `;
    
    groupList.appendChild(groupItem);
  });
  
  groupElement.appendChild(groupList);
}

/**
 * Updates the selection model visualization.
 * @param {Object} selectionData Current selection state data
 */
function updateSelectionModel(selectionData) {
  const selectionElement = document.getElementById('selection-model');
  selectionElement.innerHTML = '';

  const selectedCount = document.createElement('div');
  selectedCount.textContent = `Selected Tabs: ${selectionData.selected_count}`;
  selectionElement.appendChild(selectedCount);

  const selectionList = document.createElement('ul');
  selectionList.className = 'selection-list';
  selectionData.selection.forEach(tab => {
    const selectionItem = document.createElement('li');
    selectionItem.className = 'selection-item';
    if (tab.is_active) {
      selectionItem.classList.add('active');
    }
    
    selectionItem.innerHTML = `
      <div class="selection-title">${tab.title}</div>
      <div class="selection-meta">
        Index: ${tab.index}
        ${tab.is_active ? '(Active)' : ''}
      </div>
    `;
    
    selectionList.appendChild(selectionItem);
  });
  
  selectionElement.appendChild(selectionList);
}

/**
 * Updates the session information display.
 * @param {Object} sessionData Session restore data
 */
function updateSessionInfo(sessionData) {
  const sessionElement = document.getElementById('session-info');
  sessionElement.innerHTML = '';

  const sessionCount = document.createElement('div');
  sessionCount.textContent = `Total Sessions: ${sessionData.session_count}`;
  sessionElement.appendChild(sessionCount);

  const sessionList = document.createElement('ul');
  sessionList.className = 'session-list';
  sessionData.sessions.forEach(session => {
    const sessionItem = document.createElement('li');
    sessionItem.className = 'session-item';
    
    sessionItem.innerHTML = `
      <div class="session-id">Session ID: ${session.session_id}</div>
    `;
    
    sessionList.appendChild(sessionItem);
  });
  
  sessionElement.appendChild(sessionList);
}

/** Main initialization */
// Add listeners for WebUI events
cr.addWebUIListener('tab-strip-state-changed',
    (jsonState) => updateTabstripTree(JSON.parse(jsonState)));
cr.addWebUIListener('group-state-changed',
    (jsonState) => updateGroupInfo(JSON.parse(jsonState)));
cr.addWebUIListener('selection-state-changed',
    (jsonState) => updateSelectionModel(JSON.parse(jsonState)));
cr.addWebUIListener('session-state-changed',
    (jsonState) => updateSessionInfo(JSON.parse(jsonState)));

document.addEventListener('DOMContentLoaded', function() {
  chrome.send('initialize');
});