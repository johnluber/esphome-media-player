import { defineAsyncComponent, h } from 'vue'
import DefaultTheme from 'vitepress/theme'
import SupportButton from './components/SupportButton.vue'
import './style.css'

export default {
  extends: DefaultTheme,
  Layout() {
    return h(DefaultTheme.Layout, null, {
      'layout-bottom': () => h(SupportButton),
    })
  },
  enhanceApp({ app }) {
    app.component('InstallButton', defineAsyncComponent(() => import('./components/InstallButton.vue')))
  },
}
