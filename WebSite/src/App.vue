<script setup lang="ts">
import { ref } from 'vue'
const menuOpen = ref(false)
const email = ref('')
const submitted = ref(false)
const closeMenu = () => { menuOpen.value = false }
const submitEmail = () => { if (email.value.trim()) submitted.value = true }
</script>

<template>
  <div class="site-shell">
    <header class="site-header">
      <a class="brand" href="#" aria-label="FoRgE home"><img src="/forge-mark.svg" alt="" class="brand-icon" /><span>FoRgE</span></a>
      <button class="menu-toggle" @click="menuOpen = !menuOpen" :aria-expanded="menuOpen" aria-label="Toggle navigation"><i></i><i></i></button>
      <nav :class="{ 'nav-open': menuOpen }">
        <a href="#platform" @click="closeMenu">Platform</a><a href="#workflow" @click="closeMenu">How it works</a><a href="#worlds" @click="closeMenu">Games</a><a href="#about" @click="closeMenu">About</a><a class="nav-cta" href="#access" @click="closeMenu">Get updates <span>↗</span></a>
      </nav>
    </header>
    <main>
      <section class="hero">
        <div class="hero-copy">
          <div class="overline"><span class="status-dot"></span> AN OPEN PLATFORM FOR PLAYER-MADE GAMES</div>
          <h1>PLAY IT.<br /><span>CHANGE IT.</span><br />MAKE IT YOURS.</h1>
          <p class="hero-intro">A home for player-made worlds. Discover games, open the workshop, build with your own tools, and publish something other people can jump into.</p>
          <div class="hero-actions"><a class="button button-main" href="#platform">TAKE A LOOK <span>↗</span></a><a class="quiet-link" href="#workflow">WHAT IS FoRgE? <span>↓</span></a></div>
          <div class="hero-note"><span>01</span> A PLATFORM IN THE MAKING <i></i> BUILT FOR PLAYERS AND CREATORS</div>
        </div>
        <div class="product-window" aria-label="FoRgE platform interface preview">
          <div class="window-top"><div class="window-brand"><img src="/forge-mark.svg" alt="" /><b>FoRgE</b><span>/</span><small>DISCOVER</small></div><div class="window-controls"><i></i><i></i><i></i></div></div>
          <div class="window-body">
            <aside class="window-sidebar"><span class="side-label">LIBRARY</span><span class="side-item active"><b>◈</b> Discover</span><span class="side-item"><b>▣</b> My games</span><span class="side-item"><b>⌘</b> Workshop</span><div class="sidebar-bottom"><span class="avatar">F</span><span><b>YOUR SPACE</b><small>Creator profile</small></span></div></aside>
            <div class="window-main">
              <div class="window-heading"><div><small>YOUR NEXT SESSION STARTS HERE</small><h3>Find your next world.</h3></div><div class="window-wallet"><span class="forge-coin">\uE001</span><b>2,450</b><small>FORGE COINS</small></div><span class="window-filter">ALL GAMES&nbsp;⌄</span></div>
              <div class="featured-game"><div class="featured-art"><div class="art-moon"></div><div class="art-ridge ridge-one"></div><div class="art-ridge ridge-two"></div><span>FEATURED WORLD / 001</span></div><div class="featured-info"><div><small>EXPLORATION · COMMUNITY</small><b>Somewhere, not nowhere.</b></div><span class="play-button">PLAY&nbsp; ↗</span></div></div>
              <div class="window-section-title"><b>MADE BY THE COMMUNITY</b><span>VIEW ALL ↗</span></div>
              <div class="mini-games"><div class="mini-game"><div class="mini-art mini-art-one"><i></i></div><b>Dust District</b><small>WORLD · 12 MIN AGO</small></div><div class="mini-game"><div class="mini-art mini-art-two"><i></i></div><b>Low Orbit</b><small>EXPERIMENT · 1 HOUR AGO</small></div><div class="mini-game"><div class="mini-art mini-art-three"><i></i></div><b>Night Shift</b><small>RACING · YESTERDAY</small></div></div>
              <div class="window-foot"><span><i></i> PREVIEW CONCEPT</span><span>INTERFACE SUBJECT TO CHANGE</span></div>
            </div>
          </div>
        </div>
        <div class="hero-side-label">FIG. 01 — THE FoRgE DESKTOP EXPERIENCE</div>
      </section>
      <div class="ticker"><div class="ticker-track"><span>DISCOVER WORLDS</span><b>✳</b><span>MAKE SOMETHING NEW</span><b>✳</b><span>PLAY YOUR WAY</span><b>✳</b><span>DISCOVER WORLDS</span><b>✳</b><span>MAKE SOMETHING NEW</span><b>✳</b><span>PLAY YOUR WAY</span><b>✳</b></div></div>
      <section id="platform" class="section-wrap">
        <div class="section-topline"><span>01 / THE PLATFORM</span><span>MORE THAN A GAME LIBRARY</span></div>
        <div class="platform-heading"><h2>ONE PLACE.<br /><em>THREE WAYS TO PLAY.</em></h2><p>Jump into something someone else made. Tweak an idea until it feels right. Or start with an empty project and see where it goes.</p></div>
        <div class="pillars">
          <article class="pillar"><div class="pillar-number">01 <span>— DISCOVER</span></div><div class="pillar-graphic discover-graphic"><div class="disc-sun"></div><div class="disc-horizon"></div><div class="disc-tower"></div><span>FIND A WORLD</span></div><h3>FIND YOUR NEXT<br />FAVOURITE.</h3><p>Browse experiences made by other players. There’s always another world worth a visit.</p></article>
          <article class="pillar"><div class="pillar-number">02 <span>— CREATE</span></div><div class="pillar-graphic create-graphic"><div class="create-grid"></div><div class="create-cube"></div><span>START BUILDING</span></div><h3>MAKE THE THING<br />YOU WISH EXISTED.</h3><p>Build scenes, test mechanics, and shape a playable world with creator-first tools.</p></article>
          <article class="pillar"><div class="pillar-number">03 <span>— SHARE</span></div><div class="pillar-graphic share-graphic"><div class="share-orbit"></div><div class="share-core">F</div><span>PUT IT OUT THERE</span></div><h3>YOUR GAME.<br />YOUR PEOPLE.</h3><p>Share what you make and bring other players into the worlds you create.</p></article>
        </div>
      </section>
      <section id="workflow" class="workflow-section"><div class="workflow-inner">
        <div class="workflow-visual"><div class="visual-topline"><span>WORKSHOP / BUILD 0001</span><span>01—03</span></div><div class="workbench"><div class="workbench-sidebar"><i></i><i></i><i></i><i></i></div><div class="workbench-canvas"><div class="canvas-cross cross-left">+</div><div class="canvas-cross cross-right">+</div><div class="canvas-shape"></div><div class="canvas-axis axis-x"></div><div class="canvas-axis axis-y"></div><span class="canvas-coord">X 024.8 / Y 011.2</span></div><div class="workbench-properties"><span>OBJECT</span><i></i><i></i><i></i><span>TRANSFORM</span><i></i><i></i></div></div><div class="visual-caption"><span>CONCEPT ART / NOT A FINAL SCREEN</span><span>BUILT TO BE EXPLORED ↗</span></div></div>
        <div class="workflow-copy"><div class="section-topline"><span>02 / THE WORKFLOW</span></div><h2>FROM FIRST<br />IDEA TO <em>PLAYABLE.</em></h2><p>Making a game should feel like making something — not filling out paperwork. FoRgE is being designed to bring the player and creator sides closer together.</p><div class="steps"><div><span>01</span><section><b>START WITH AN IDEA</b><small>A sketch, a mechanic, a strange little world. Start anywhere.</small></section><i>↗</i></div><div><span>02</span><section><b>BUILD AND EXPERIMENT</b><small>Shape the experience, test it, break it, make it better.</small></section><i>↗</i></div><div><span>03</span><section><b>LET PEOPLE PLAY</b><small>Share your work and see what the community does with it.</small></section><i>↗</i></div></div></div>
      </div></section>
      <section id="worlds" class="section-wrap games-section">
        <div class="section-topline"><span>03 / THE GAMES</span><span>A FEW DIRECTIONS WE LOVE</span></div>
        <div class="games-heading"><h2>NOT ONE GENRE.<br /><em>NOT ONE WAY TO PLAY.</em></h2><p>Different people make different things. That’s the point. These are visual directions for the kind of variety we want FoRgE to make room for.</p></div>
        <div class="game-strips">
          <article class="game-strip"><div class="strip-art strip-city"><i></i><b></b></div><div class="strip-index">01</div><div class="strip-copy"><small>URBAN / STORY</small><h3>AFTER THE LAST TRAIN</h3><p>A city that keeps moving after everyone else has gone home.</p></div><span class="strip-arrow">↗</span></article>
          <article class="game-strip"><div class="strip-art strip-wild"><i></i><b></b></div><div class="strip-index">02</div><div class="strip-copy"><small>EXPLORATION / SURVIVAL</small><h3>THE LONG WAY OUT</h3><p>Unmapped places, scarce resources, and no set route home.</p></div><span class="strip-arrow">↗</span></article>
          <article class="game-strip"><div class="strip-art strip-speed"><i></i><b></b></div><div class="strip-index">03</div><div class="strip-copy"><small>ARCADE / COMPETITIVE</small><h3>REDLINE CLUB</h3><p>Short races, close calls, and one more run before you quit.</p></div><span class="strip-arrow">↗</span></article>
        </div>
        <p class="concept-note">THE TITLES AND ART ABOVE ARE CONCEPTS, NOT RELEASED GAMES.</p>
      </section>
      <section id="about" class="manifesto-section"><div class="manifesto-mark"><img src="/forge-mark.svg" alt="FoRgE symbol" /></div><div class="manifesto-copy"><div class="section-topline"><span>04 / WHY FoRgE</span></div><h2>GAMES ARE BETTER<br />WHEN YOU CAN <em>MAKE THEM.</em></h2><p>We’re building FoRgE for people who don’t stop at the credits. The curious players, the tinkerers, the ones with a game idea they can’t quite let go of.</p><p class="manifesto-small">No grand promises. Just a platform we want to make worth opening.</p></div><div class="manifesto-aside"><span>BUILT AROUND<br />THE PEOPLE<br />WHO PLAY.</span><i>FoRgE / 2026</i></div></section>
      <section id="access" class="access-section"><div class="access-inner"><div class="section-topline"><span>05 / STAY IN THE LOOP</span><span>FoRgE — IN DEVELOPMENT</span></div><h2>WE’RE STILL<br /><em>MAKING THE TOOLS.</em></h2><p>FoRgE is in development. Leave your email if you want to hear when there’s something real to try.</p><form class="access-form" @submit.prevent="submitEmail"><input v-model="email" type="email" placeholder="Email address" aria-label="Email address" required /><button type="submit">{{ submitted ? 'THANK YOU ✓' : 'KEEP ME POSTED ↗' }}</button></form><small class="form-note">{{ submitted ? 'Thanks — this demo form does not send or store your address yet.' : 'NO FAKE LAUNCH DATE. JUST AN UPDATE WHEN THERE IS NEWS.' }}</small></div></section>
    </main>
    <footer class="site-footer"><a class="brand" href="#"><img src="/forge-mark.svg" alt="" class="brand-icon" /><span>FoRgE</span></a><span class="footer-line">PLAY IT. CHANGE IT. MAKE IT YOURS.</span><span class="footer-copy">© 2026 FoRgE — MADE FOR THE CURIOUS.</span><a class="back-top" href="#">BACK TO TOP ↑</a></footer>
  </div>
</template>
