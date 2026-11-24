<script setup lang="ts">
import { ref } from 'vue'

const menuOpen = ref(false)
const email = ref('')
const subscribed = ref(false)

const closeMenu = () => { menuOpen.value = false }
const joinWaitlist = () => {
  if (email.value.trim()) subscribed.value = true
}
</script>

<template>
  <div class="site-shell">
    <div class="topline"><span class="topline-dot"></span> THE NEXT GENERATION OF PLAYER-CREATED GAMES</div>
    <header class="nav-wrap">
      <a class="brand" href="#" aria-label="FoRgE home">
        <svg class="brand-mark" viewBox="0 0 54 54" fill="none" aria-hidden="true">
          <path d="M8 7H47V18H20V25H43V36H20V48H8V7Z" fill="currentColor"/>
          <path d="M8 7H20V48H8V7Z" fill="#E3AD83"/>
          <path d="M34 7H47V18H34V7Z" fill="#8D583B"/>
        </svg>
        <span>Fo<span>R</span>GE</span>
      </a>
      <button class="menu-toggle" @click="menuOpen = !menuOpen" :aria-expanded="menuOpen" aria-label="Toggle navigation"><i></i><i></i></button>
      <nav :class="{ 'nav-open': menuOpen }">
        <a href="#discover" @click="closeMenu">Discover</a>
        <a href="#create" @click="closeMenu">Create</a>
        <a href="#community" @click="closeMenu">Community</a>
        <a href="#about" @click="closeMenu">About</a>
        <a class="nav-cta" href="#get-started" @click="closeMenu">Get early access <span>↗</span></a>
      </nav>
    </header>

    <main>
      <section class="hero">
        <div class="hero-grid"></div>
        <div class="hero-copy">
          <div class="eyebrow"><span class="eyebrow-line"></span> YOUR WORLD. YOUR RULES.</div>
          <h1>MAKE WORLDS.<br><span>PLAY ANYTHING.</span></h1>
          <p class="hero-lead">A universe built by players, powered by creators. Build your own games, share them with the world, and jump into experiences you’ve never imagined.</p>
          <div class="hero-actions">
            <a href="#get-started" class="button button-primary">ENTER THE FORGE <span>↗</span></a>
            <a href="#create" class="button button-quiet"><span class="play-icon">▶</span> HOW IT WORKS</a>
          </div>
          <div class="hero-footnote"><span class="pulse"></span> A NEW WORLD IS WAITING TO BE BUILT</div>
        </div>
        <div class="hero-art" aria-label="Abstract copper 3D forge emblem">
          <div class="orbit orbit-one"></div><div class="orbit orbit-two"></div>
          <div class="hero-cross">✳</div>
          <div class="forge-object">
            <div class="object-top"></div><div class="object-front"></div><div class="object-side"></div>
            <div class="object-cut"></div>
          </div>
          <div class="art-label art-label-top">FIG. 001 <span>— THE CREATOR ENGINE</span></div>
          <div class="art-label art-label-bottom"><span class="art-square"></span> BUILT DIFFERENT. BUILT TOGETHER.</div>
          <div class="art-coordinate">51°30'26.4"N<br>00°07'39.0"W</div>
        </div>
        <div class="hero-index">01 <span>/ 04</span></div>
      </section>

      <section class="ticker" aria-label="Platform features">
        <div class="ticker-track"><span>CREATE WITHOUT LIMITS</span><b>✳</b><span>PLAY WITHOUT BORDERS</span><b>✳</b><span>BUILT BY THE COMMUNITY</span><b>✳</b><span>CREATE WITHOUT LIMITS</span><b>✳</b><span>PLAY WITHOUT BORDERS</span><b>✳</b></div>
      </section>

      <section id="discover" class="section discover-section">
        <div class="section-kicker">01 / THE UNIVERSE</div>
        <div class="section-heading-row">
          <h2>ONE PLATFORM.<br><em>INFINITE WORLDS.</em></h2>
          <p>There’s always something new around the corner. Explore player-made worlds, find your next obsession, or bring your crew along for the ride.</p>
        </div>
        <div class="world-grid">
          <article class="world-card world-large">
            <div class="world-visual visual-neon"><div class="neon-sun"></div><div class="neon-city"></div><span class="visual-tag">01 / ADVENTURE</span></div>
            <div class="world-meta"><div><h3>AFTER HOURS</h3><p>Explore the city after dark.</p></div><span class="arrow-round">↗</span></div>
          </article>
          <article class="world-card">
            <div class="world-visual visual-dunes"><div class="dune dune-a"></div><div class="dune dune-b"></div><div class="dune dune-c"></div><span class="visual-tag">02 / SURVIVAL</span></div>
            <div class="world-meta"><div><h3>LAST FRONTIER</h3><p>Survive. Adapt. Build.</p></div><span class="arrow-round">↗</span></div>
          </article>
          <article class="world-card">
            <div class="world-visual visual-arcade"><div class="arcade-ring"></div><div class="arcade-cube"></div><span class="visual-tag">03 / COMPETITIVE</span></div>
            <div class="world-meta"><div><h3>OVERDRIVE</h3><p>Only the sharpest survive.</p></div><span class="arrow-round">↗</span></div>
          </article>
        </div>
        <div class="coming-note"><span>MORE WORLDS ARE IN THE WORKS</span><span>THE LIMIT IS YOUR IMAGINATION ↗</span></div>
      </section>

      <section id="create" class="create-section">
        <div class="create-art">
          <div class="blueprint-label">CREATOR TOOLKIT / 001</div>
          <div class="blueprint-frame">
            <div class="blueprint-cross cross-a">+</div><div class="blueprint-cross cross-b">+</div>
            <div class="blueprint-cube"><span></span><i></i><b></b></div>
            <div class="blueprint-measure measure-x">120.00</div><div class="blueprint-measure measure-y">080.00</div>
          </div>
          <div class="blueprint-caption">FROM A BLANK CANVAS<br>TO A LIVING WORLD.</div>
        </div>
        <div class="create-copy">
          <div class="section-kicker">02 / THE WORKSHOP</div>
          <h2>DON’T JUST<br>PLAY THE GAME.<br><em>MAKE IT.</em></h2>
          <p>Got an idea? Make it real. FoRgE gives creators the space to turn imagination into interactive experiences and put them in front of a global community.</p>
          <ul class="feature-list">
            <li><span>01</span><div><strong>BUILD YOUR WAY</strong><small>Bring your ideas to life with creator-friendly tools.</small></div><b>↗</b></li>
            <li><span>02</span><div><strong>PUBLISH INSTANTLY</strong><small>Share your world and invite everyone in.</small></div><b>↗</b></li>
            <li><span>03</span><div><strong>GROW TOGETHER</strong><small>Get discovered, find collaborators, build a following.</small></div><b>↗</b></li>
          </ul>
          <a href="#get-started" class="text-link">BECOME A CREATOR <span>↗</span></a>
        </div>
      </section>

      <section id="community" class="community-section">
        <div class="section-kicker">03 / THE PEOPLE</div>
        <div class="community-content">
          <div><h2>MADE BY MANY.<br><em>PLAYED BY EVERYONE.</em></h2><p>FoRgE is more than a place to play. It’s where creators meet players, small ideas become big adventures, and communities make their own rules.</p></div>
          <div class="community-stamp"><span>F</span><small>CREATE<br>TOGETHER<br>★ 2026 ★</small></div>
        </div>
        <div class="community-bottom"><span>CREATORS WELCOME. ALWAYS.</span><span>NO GATES. JUST POSSIBILITIES.</span></div>
      </section>

      <section id="get-started" class="signup-section">
        <div class="signup-mark">F.</div>
        <div class="section-kicker">04 / YOUR MOVE</div>
        <h2>THE NEXT WORLD<br><em>COULD BE YOURS.</em></h2>
        <p>We’re getting the forge ready. Get on the list and be among the first to step inside.</p>
        <form class="signup-form" @submit.prevent="joinWaitlist">
          <input v-model="email" type="email" placeholder="Your email address" aria-label="Your email address" required />
          <button type="submit">{{ subscribed ? 'YOU’RE ON THE LIST ✓' : 'GET EARLY ACCESS ↗' }}</button>
        </form>
        <small class="form-note">{{ subscribed ? 'Thanks for your interest. FoRgE updates are coming.' : 'NO SPAM. JUST FORGE UPDATES.' }}</small>
      </section>
    </main>

    <footer id="about" class="footer">
      <a class="brand footer-brand" href="#"><svg class="brand-mark" viewBox="0 0 54 54" fill="none"><path d="M8 7H47V18H20V25H43V36H20V48H8V7Z" fill="currentColor"/><path d="M8 7H20V48H8V7Z" fill="#E3AD83"/><path d="M34 7H47V18H34V7Z" fill="#8D583B"/></svg><span>Fo<span>R</span>GE</span></a>
      <span class="footer-tagline">MAKE WORLDS. PLAY ANYTHING.</span>
      <span class="copyright">© 2026 FoRgE. ALL WORLDS RESERVED.</span>
      <a class="back-top" href="#">BACK TO TOP ↑</a>
    </footer>
  </div>
</template>