import { PolymerElement, html } from '//resources/polymer/v3_0/polymer/polymer_bundled.min.js';
import '//resources/polymer/v3_0/paper-card/paper-card.js';

class DebugTabstrip extends PolymerElement {
  static get is() { return 'debug-tabstrip'; }

  static get template() {
    return html`
      <style>
        :host {
          display: block;
          padding: 16px;
          font-family: Arial, sans-serif;
        }
        .tab-group {
          margin-bottom: 16px;
          padding: 8px;
          border: 1px solid #ccc;
          border-radius: 4px;
          background-color: #f9f9f9;
        }
        .tab {
          margin: 4px 0;
          padding: 4px;
          background: #fff;
          border-radius: 2px;
          box-shadow: 1px 1px 4px rgba(0, 0, 0, 0.1);
        }
      </style>
      <h2>Tabstrip Debugger</h2>
      <template is="dom-repeat" items="[[tabGroups]]" as="group">
        <div class="tab-group">
          <h3>Group: [[group.name]]</h3>
          <template is="dom-repeat" items="[[group.tabs]]" as="tab">
            <div class="tab">[[tab.title]]</div>
          </template>
        </div>
      </template>
    `;
  }

  static get properties() {
    return {
      tabGroups: {
        type: Array,
        value: () => [],
      },
    };
  }

  connectedCallback() {
    super.connectedCallback();
    this.fetchTabData();
  }

  async fetchTabData() {
    try {
      const response = await fetch('chrome://tabstrip-debug-data');
      const data = await response.json();
      this.tabGroups = data.groups || [];
    } catch (error) {
      console.error('Failed to fetch tabstrip data:', error);
    }
  }
}

customElements.define(DebugTabstrip.is, DebugTabstrip);
