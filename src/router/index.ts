import { createRouter, createWebHistory } from 'vue-router'
import LoginView from '../views/LoginView.vue'
import IndexView from '../views/IndexView.vue'
import RegisterView from '../views/RegisterView.vue'
import recv_tmp from '@/views/recv_tmp.vue'
const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: '/login',
      name: 'login',
      component: LoginView
    },
    {
      path: '/register',
      name: 'register',
      component: RegisterView
    },
    {
      path: '/',
      name: 'home',
      component: IndexView
    },
    {
      path: '/test',
      name: 'test',
      component: recv_tmp
    }
  ],
})

export default router
