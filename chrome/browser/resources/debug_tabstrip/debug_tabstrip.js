import { html, css, LitElement } from 'lit';

class TabstripDebugUI extends LitElement {
  static styles = css`
    :host { font-family: Arial, sans-serif; display: block; padding: 20px; }
    .tab { margin-bottom: 10px; padding: 10px; border: 1px solid #ddd; border-radius: 5px; }
    .active { background-color: #f0f8ff; font-weight: bold; }
    .group { margin-top: 15px; padding: 10px; border: 2px solid #aaa; border-radius: 5px; }
    button { padding: 5px 10px; margin: 10px 0; cursor: pointer; }
  `;

  static properties = {
    tabs: { type: Array },
    groups: { type: Array }
  };

  constructor() {
    super();
    this.tabs = [];
    this.groups = [];
    this.fetchTabstripData();
  }

  async fetchTabstripData() {
    try {
      const response = await fetch('chrome://tabstrip-debug-data');
      const data = await response.json();
      this.tabs = Object.values(data.tabs || {});
      this.groups = Object.values(data.groups || {});
    } catch (error) {
      console.error('Error fetching tab data:', error);
    }
  }

  render() {
    return html`
      <h2>Tabstrip Debug UI</h2>
      <button @click="${this.fetchTabstripData}">Refresh Data</button>
      
      <div>
        ${this.groups.map(group => html`
          <div class="group">
            <strong>Group: ${group.title} (${group.tab_indices.length} tabs)</strong>
            <p>Color: ${group.color}</p>
          </div>
        `)}
      </div>
      
      <div>
        ${this.tabs.map(tab => html`
          <div class="tab ${tab.is_active ? 'active' : ''}">
            <strong>${tab.title}</strong> - <a href="${tab.url}" target="_blank">${tab.url}</a><br>
            <small>Index: ${tab.index} | Pinned: ${tab.is_pinned} | Loading: ${tab.is_loading}</small>
          </div>
        `)}
      </div>
    `;
  }
}

customElements.define('tabstrip-debug-ui', TabstripDebugUI);
