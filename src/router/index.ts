import { createRouter, createWebHistory } from 'vue-router'
import LoginView from '../views/qq/LoginView.vue'
import IndexView from '../views/qq/IndexView.vue'
import RegisterView from '../views/qq/RegisterView.vue'
import LoginViewWx from '@/views/wx/LoginView.vue'
import IndexViewWx from '@/views/wx/IndexView.vue'
import RegisterViewWx from '@/views/wx/RegisterView.vue'
import IndexViewWb from '@/views/wb/IndexView.vue'
import RegisterViewWb from '../views/qq/RegisterView.vue'
import LoginViewWb from '../views/qq/LoginView.vue'

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: '/qq/login',
      name: 'login',
      component: LoginView
    },
    {
      path: '/qq/register',
      name: 'register',
      component: RegisterView
    },
    {
      path: '/qq',
      name: 'home',
      component: IndexView
    },
    {
      path: "/wx/login",
      name: "login_wx",
      component: LoginViewWx
    },
    {
      path: "/wx/register",
      name: "register_wx",
      component: RegisterViewWx
    },
    {
      path: "/wx",
      name: "home_wx",
      component: IndexViewWx
    },
    {
      path: "/wb",
      name: "home_wb",
      component: IndexViewWb
    },
    {
      path: "/wb/login",
      name: "home_wb",
      component: LoginViewWx
    },
    {
      path: "/wb/register",
      name: "home_wb",
      component: RegisterViewWb
    }
  ],
})

export default router
