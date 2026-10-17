
importScripts('https://www.gstatic.com/firebasejs/10.12.2/firebase-app-compat.js');
importScripts('https://www.gstatic.com/firebasejs/10.12.2/firebase-messaging-compat.js');

firebase.initializeApp({
  apiKey: "AIzaSyCriW12_vEx_dwGAj6D99XUg_w1ikdGur4",
  authDomain: "smart-home-c853b.firebaseapp.com",
  databaseURL: "https://smart-home-c853b-default-rtdb.firebaseio.com",
  projectId: "smart-home-c853b",
  storageBucket: "smart-home-c853b.firebasestorage.app",
  messagingSenderId: "932456433882",
  appId: "1:932456433882:web:e93f2806ff8b8090167de3",
});

const messaging = firebase.messaging();

messaging.onBackgroundMessage((payload) => {
  const title = payload.notification?.title || "RENO Alert";
  const options = {
    body: payload.notification?.body || "Check your sensor readings.",
    icon: "/favicon.ico",
    data: payload.data || {}
  };

  self.registration.showNotification(title, options);
});
