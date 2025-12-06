import {
  rtdb, ref, onValue,
  firestore, collection, addDoc, onSnapshot, query, orderBy
} from "./firebase.js";
console.log("[app.js] Loaded for page:", document.body.dataset.current);
document.addEventListener("DOMContentLoaded", () => {
  const page = document.body.dataset.current;

  if (page === "notifications") initNotificationsPage();
  if (page === "visitors") initVisitorsPage();
  if (page === "home") initHomePage();
});

/* ------------------------------
   HOME PAGE (TEMP + HUMID)
--------------------------------*/
function initHomePage() {
  const tempEl = document.querySelector("[data-temp]");
  const humEl = document.querySelector("[data-hum]");

  onValue(ref(rtdb, "sensor/temp"), snap => {
    if (snap.exists()) tempEl.textContent = snap.val();
  });

  onValue(ref(rtdb, "sensor/hum"), snap => {
    if (snap.exists()) humEl.textContent = snap.val();
  });
}

/* ------------------------------
   NOTIFICATIONS PAGE
--------------------------------*/
function initNotificationsPage() {
  const notifList = document.querySelector(".notif-list");
  notifList.innerHTML = ""; // remove template cards

  // Listen for live alert updates
  onValue(ref(rtdb, "sensor"), (snap) => {
    if (!snap.exists()) return;

    const sensor = snap.val();
    console.log("Realtime sensor:", sensor);

    notifList.innerHTML = ""; // Clear old cards

    if (sensor.fireDanger == 1) addAlert("Fire Alarm", "./Resource/fire.svg", "anim-flicker");
    if (sensor.gasDanger == 1) addAlert("Carbon Monoxide Alarm", "./Resource/co.svg", "anim-bob");

    if (sensor.fireDanger == 0 && sensor.gasDanger == 0) {
      notifList.innerHTML = `<p class="text-gray-600">No fire or gas danger.</p>`;
    }
  });

  function addAlert(title, iconSrc, animClass) {
    const now = new Date();
    const timeStr = now.toLocaleDateString() + " | " +
      now.toLocaleTimeString([], { hour: "2-digit", minute: "2-digit" });

    const card = document.createElement("article");
    card.className = "card notif reveal hover-float in-view";
    card.innerHTML = `
      <img class="icon ${animClass}" alt="${title}" src="${iconSrc}" />
      <div>
        <p class="notif-title">${title}</p>
        <p class="notif-sub">${timeStr}</p>
      </div>
    `;
    notifList.prepend(card);
  }
}

/* ------------------------------
   VISITORS PAGE (Motion Events)
--------------------------------*/
function initVisitorsPage() {
  console.log("[Visitors] Initializing Firestore listener...");

  const visitList = document.querySelector(".visit-list");
  const filterDate = document.getElementById('filter-date');
  const filterTimeStart = document.getElementById('filter-time-start');
  const filterTimeEnd = document.getElementById('filter-time-end');
  const clearFiltersBtn = document.getElementById('clear-filters');

  let allEvents = []; // Store all events for filtering

  const motionEventsRef = collection(firestore, "motion_events");
  const qMotionEvents = query(motionEventsRef, orderBy("timestamp", "desc"));

  // Listen to Firestore updates in realtime
  onSnapshot(qMotionEvents, (snapshot) => {
    console.log("[Motion Events] Snapshot received:", snapshot.size, "entries");
    
    // Store all events
    allEvents = [];
    snapshot.forEach((doc) => {
      allEvents.push({ id: doc.id, data: doc.data() });
    });

    // Render with current filters
    renderEvents(allEvents);
  });

  // Filter event listeners
  filterDate.addEventListener('change', () => renderEvents(allEvents));
  filterTimeStart.addEventListener('change', () => renderEvents(allEvents));
  filterTimeEnd.addEventListener('change', () => renderEvents(allEvents));
  
  clearFiltersBtn.addEventListener('click', () => {
    filterDate.value = '';
    filterTimeStart.value = '';
    filterTimeEnd.value = '';
    renderEvents(allEvents);
  });

  function renderEvents(events) {
    visitList.innerHTML = ""; // clear existing cards

    const filteredEvents = events.filter((event) => {
      const data = event.data;
      const timestamp = data.timestamp;
      
      // Handle Unix timestamp (seconds) - multiply by 1000 to get milliseconds
      const dateObj = new Date(timestamp * 1000);

      // Date filter
      if (filterDate.value) {
        const filterDateObj = new Date(filterDate.value);
        const eventDate = dateObj.toISOString().split('T')[0];
        const filterDateStr = filterDateObj.toISOString().split('T')[0];
        if (eventDate !== filterDateStr) return false;
      }

      // Time range filter
      if (filterTimeStart.value || filterTimeEnd.value) {
        const eventTime = dateObj.getHours() * 60 + dateObj.getMinutes();
        
        if (filterTimeStart.value) {
          const [startH, startM] = filterTimeStart.value.split(':').map(Number);
          const startTime = startH * 60 + startM;
          if (eventTime < startTime) return false;
        }
        
        if (filterTimeEnd.value) {
          const [endH, endM] = filterTimeEnd.value.split(':').map(Number);
          const endTime = endH * 60 + endM;
          if (eventTime > endTime) return false;
        }
      }

      return true;
    });

    filteredEvents.forEach((event) => {
      const data = event.data;
      const timestamp = data.timestamp;
      const base64Image = data.base64Image || "";
      const size = data.size || { width: 160, height: 120 };

      // Handle Unix timestamp (seconds) - multiply by 1000 to get milliseconds
      const dateObj = new Date(timestamp * 1000);
      const dateStr = dateObj.toLocaleDateString();
      const timeStr = dateObj.toLocaleTimeString([], { hour: "2-digit", minute: "2-digit" });

      const card = document.createElement("article");
      card.className = "card visit-card reveal hover-float in-view";

      card.innerHTML = `
        <header class="visit-header">
          <div>
            <p class="visit-date">${dateStr}</p>
            <p class="visit-time">${timeStr}</p>
          </div>
        </header>

        <div class="single-thumb">
          <canvas class="thumb-canvas"></canvas>
        </div>
      `;

      const thumbEl = card.querySelector(".single-thumb");
      const canvas = card.querySelector(".thumb-canvas");

      if (base64Image) {
        try {
          const binaryString = atob(base64Image);
          const bytes = new Uint8Array(binaryString.length);
          for (let i = 0; i < binaryString.length; i++) {
            bytes[i] = binaryString.charCodeAt(i);
          }

          const width = size.width || 160;
          const height = size.height || 120;
          canvas.width = width;
          canvas.height = height;

          const ctx = canvas.getContext("2d");
          const imageData = ctx.createImageData(width, height);
          
          for (let i = 0; i < bytes.length; i++) {
            const grayscale = bytes[i];
            const pixelIndex = i * 4;
            imageData.data[pixelIndex] = grayscale;
            imageData.data[pixelIndex + 1] = grayscale;
            imageData.data[pixelIndex + 2] = grayscale;
            imageData.data[pixelIndex + 3] = 255;
          }

          ctx.putImageData(imageData, 0, 0);

          canvas.style.width = "100%";
          canvas.style.height = "100%";
          canvas.style.objectFit = "cover";

          console.log(`Motion event ${event.id} decoded successfully`);
        } catch (error) {
          console.error(`Failed to decode image for ${event.id}:`, error);
          thumbEl.style.backgroundColor = "#f3f6fb";
          thumbEl.innerHTML = '<p style="color: #94a3b8; font-size: 11px; text-align: center; margin-top: 35px;">Failed to load</p>';
        }
      } else {
        thumbEl.style.backgroundColor = "#f3f6fb";
        thumbEl.innerHTML = '<p style="color: #94a3b8; font-size: 11px; text-align: center; margin-top: 35px;">No image</p>';
      }

      visitList.appendChild(card);
    });

    if (filteredEvents.length === 0) {
      visitList.innerHTML = '<p style="color: var(--muted); text-align: center; padding: 2rem;">No motion events found for selected filters.</p>';
    }
  }
}