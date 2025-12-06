
// firebase.js
import { initializeApp } from "https://www.gstatic.com/firebasejs/10.14.0/firebase-app.js";

// RTDB
import {
  getDatabase,
  ref,
  onValue,
  push,
  set
} from "https://www.gstatic.com/firebasejs/10.14.0/firebase-database.js";

// Firestore
import {
  getFirestore,
  collection,
  addDoc,
  onSnapshot,
  query,
  orderBy
} from "https://www.gstatic.com/firebasejs/10.14.0/firebase-firestore.js";

const firebaseConfig = {
  apiKey: "AIzaSyDjuZu6T2rRkuGBGkx37qRWb_Ifdb8RYBQ",
  authDomain: "embedded-system-project-e1291.firebaseapp.com",
  databaseURL: "https://embedded-system-project-e1291-default-rtdb.asia-southeast1.firebasedatabase.app/",
  projectId: "embedded-system-project-e1291",
  storageBucket: "embedded-system-project-e1291.firebasestorage.app",
  messagingSenderId: "312814327748",
  appId: "1:312814327748:web:173fa0269ac229f6dff691"
};

const app = initializeApp(firebaseConfig);

const rtdb = getDatabase(app);
const firestore = getFirestore(app);

export {
  rtdb, ref, onValue, push, set,
  firestore, collection, addDoc, onSnapshot, query, orderBy
};
