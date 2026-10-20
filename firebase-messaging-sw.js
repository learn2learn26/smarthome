
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
  measurementId: "G-SWN5NQRBVN"
});

const messaging = firebase.messaging();

messaging.onBackgroundMessage((payload) => {
  const title = payload.notification?.title || "RENO Alert";

  self.registration.showNotification(title, {
    body: payload.notification?.body || "Check your sensor readings.",
    icon: "/smarthome/favicon.ico",
    data: {
      url: "https://learn2learn26.github.io/smarthome/"
    }
  });
});

self.addEventListener("notificationclick", (event) => {
  event.notification.close();

  const targetUrl = "https://learn2learn26.github.io/smarthome/";

  event.waitUntil(
    clients.matchAll({
      type: "window",
      includeUncontrolled: true
    }).then((windowClients) => {
      for (const client of windowClients) {
        if (client.url.startsWith(targetUrl) && "focus" in client) {
          return client.focus();
        }
      }

      return clients.openWindow(targetUrl);
    })
  );
});
