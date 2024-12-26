/** @type {import('tailwindcss').Config} */
export default {
  purge: ['./src/index.html', './src/**/*.{vue,js,ts,jsx,tsx}'],
  content: ["./src/**/*.html", "./src/**/*.vue"],
  theme: {
    extend: {},
  },
  plugins: [],
}

