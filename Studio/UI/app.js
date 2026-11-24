import * as THREE from 'three';
import { OrbitControls } from './OrbitControls.js';
const root=document.querySelector('#viewport');
const scene=new THREE.Scene();scene.background=new THREE.Color(0x202729);scene.fog=new THREE.Fog(0x202729,24,70);
const camera=new THREE.PerspectiveCamera(48,1,.1,150);camera.position.set(10,10,13);
const renderer=new THREE.WebGLRenderer({antialias:true,powerPreference:'high-performance'});renderer.setPixelRatio(Math.min(devicePixelRatio,2));renderer.shadowMap.enabled=true;renderer.shadowMap.type=THREE.PCFSoftShadowMap;renderer.outputColorSpace=THREE.SRGBColorSpace;root.appendChild(renderer.domElement);
const controls=new OrbitControls(camera,renderer.domElement);controls.target.set(0,0,0);controls.enableDamping=true;controls.dampingFactor=.07;controls.maxPolarAngle=Math.PI*.48;controls.minDistance=4;controls.maxDistance=38;
scene.add(new THREE.HemisphereLight(0xdde8ee,0x45413c,2.1));const sun=new THREE.DirectionalLight(0xffe1c7,3.2);sun.position.set(8,15,9);sun.castShadow=true;sun.shadow.mapSize.set(2048,2048);scene.add(sun);
const ground=new THREE.Mesh(new THREE.PlaneGeometry(100,100),new THREE.MeshStandardMaterial({color:0x262d2e,roughness:.95}));ground.rotation.x=-Math.PI/2;ground.position.y=-.52;ground.receiveShadow=true;scene.add(ground);
const grid=new THREE.GridHelper(60,60,0x65706d,0x3d4746);grid.position.y=-.5;scene.add(grid);
const mats=[new THREE.MeshStandardMaterial({color:0xc46a4b,roughness:.58,metalness:.24}),new THREE.MeshStandardMaterial({color:0x879092,roughness:.9}),new THREE.MeshStandardMaterial({color:0x71985c,roughness:.95})];
const geometry=new THREE.BoxGeometry(1,1,1);const blocks=[];let selected=null;let activeMaterial=0;
function addBlock(x,y,z,type=activeMaterial){const mesh=new THREE.Mesh(geometry,mats[type]);mesh.position.set(x,y,z);mesh.castShadow=true;mesh.receiveShadow=true;mesh.userData={type,name:['Copper Block','Stone Block','Grass Block'][type]};scene.add(mesh);blocks.push(mesh);select(mesh);updateCount();return mesh}
function select(mesh){selected=mesh;document.querySelector('#selection').textContent=mesh?mesh.userData.name+'  ·  Block':'No object selected';document.querySelector('#object-name').value=mesh?mesh.userData.name:'—';for(const [id,v] of [['pos-x',mesh?.position.x],['pos-y',mesh?.position.y],['pos-z',mesh?.position.z]])document.getElementById(id).value=mesh?v.toFixed(2):'—'}
function updateCount(){document.querySelector('#object-count').textContent=blocks.length}
function save(){const rows=blocks.map(b=>[Math.round(b.position.x),Math.round(b.position.z),b.userData.type].join(',')).join(';');window.chrome.webview.postMessage('save|'+rows);document.querySelector('#status').textContent='Saved scene.forge · '+blocks.length+' objects'}
function play(){save();window.chrome.webview.postMessage('play')}
function removeSelected(){if(!selected)return;scene.remove(selected);const i=blocks.indexOf(selected);if(i>=0)blocks.splice(i,1);select(null);updateCount();document.querySelector('#status').textContent='Object removed'}
const raycaster=new THREE.Raycaster(),pointer=new THREE.Vector2();let pointerDown=null;
function cast(e){const r=renderer.domElement.getBoundingClientRect();pointer.set((e.clientX-r.left)/r.width*2-1,-((e.clientY-r.top)/r.height)*2+1);raycaster.setFromCamera(pointer,camera);return raycaster.intersectObjects(blocks,false)}
renderer.domElement.addEventListener('pointerdown',e=>{pointerDown={x:e.clientX,y:e.clientY,button:e.button}});
renderer.domElement.addEventListener('pointerup',e=>{if(!pointerDown||Math.hypot(e.clientX-pointerDown.x,e.clientY-pointerDown.y)>4)return;const hits=cast(e);if(e.button===2){if(hits.length){scene.remove(hits[0].object);blocks.splice(blocks.indexOf(hits[0].object),1);select(null);updateCount()}return}if(e.button!==0)return;if(hits.length){select(hits[0].object);return}const r=renderer.domElement.getBoundingClientRect();pointer.set((e.clientX-r.left)/r.width*2-1,-((e.clientY-r.top)/r.height)*2+1);raycaster.setFromCamera(pointer,camera);const floorHit=raycaster.intersectObject(ground);if(floorHit.length){const p=floorHit[0].point;addBlock(Math.round(p.x),0,Math.round(p.z))}});
renderer.domElement.addEventListener('contextmenu',e=>e.preventDefault());
document.querySelectorAll('[data-material]').forEach(b=>b.addEventListener('click',()=>{activeMaterial=Number(b.dataset.material);document.querySelectorAll('[data-material]').forEach(x=>x.classList.toggle('chosen',x===b));document.querySelector('#status').textContent='Selected material · '+['Copper','Stone','Grass'][activeMaterial]}));
document.querySelectorAll('[data-tool]').forEach(b=>b.addEventListener('click',()=>{document.querySelectorAll('[data-tool]').forEach(x=>x.classList.toggle('active',x===b));document.querySelector('#status').textContent=b.dataset.tool+' tool selected'}));
document.querySelector('#save').addEventListener('click',save);document.querySelector('#play').addEventListener('click',play);
window.addEventListener('keydown',e=>{if((e.ctrlKey||e.metaKey)&&e.key.toLowerCase()==='s'){e.preventDefault();save()}if(e.key==='Delete'||e.key==='Backspace')removeSelected();if(e.key==='Escape')select(null)});
for(let x=-3;x<=3;x++)for(let z=-3;z<=3;z++)if(Math.abs(x)+Math.abs(z)<5)addBlock(x,0,z,(x+z+9)%3);
function resize(){const w=root.clientWidth,h=root.clientHeight;renderer.setSize(w,h,false);camera.aspect=w/h;camera.updateProjectionMatrix()}new ResizeObserver(resize).observe(root);resize();
function frame(){requestAnimationFrame(frame);controls.update();renderer.render(scene,camera)}frame();