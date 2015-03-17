// Set up the scene, camera, and renderer as global variables.
var container;

var scene, camera, renderer;

var uniforms;

var cube;

init();
animate();

function init() {
    container = document.createElement('div');
    cubeContainer = document.getElementById('cube-container');
    cubeContainer.appendChild(container);

    getIt = document.getElementById('get-it');
    
    var width = getIt.clientWidth; 
    var height = 400;
        
    scene = new THREE.Scene();
        
    // Camera
    camera = new THREE.PerspectiveCamera(45, width / height, 0.1, 20000);
    camera.position.set(0,0,50);
    scene.add(camera);
    
    // Geometry
    // Shader
    var prefix = "data/sky_env_cube_";
    var envmapPaths = [ prefix + "px.png", prefix + "nx.png",
                        prefix + "py.png", prefix + "ny.png",
                        prefix + "pz.png", prefix + "nz.png" ];
    prefix = "data/gemStructure_env_cube_";
    var gemStructurePaths = [ prefix + "px.png", prefix + "nx.png",
                              prefix + "py.png", prefix + "ny.png",
                              prefix + "pz.png", prefix + "nz.png" ];
    prefix = "data/rainbow_env_cube_";
    var rainbowPaths = [ prefix + "px.png", prefix + "nx.png",
                              prefix + "py.png", prefix + "ny.png",
                              prefix + "pz.png", prefix + "nz.png" ];
                
    uniforms = {
        envMap: { type: "t", value: THREE.ImageUtils.loadTextureCube(envmapPaths) },
        gemStructureMap: { type: "t", value: THREE.ImageUtils.loadTextureCube(gemStructurePaths) },
        rainbowMap: { type: "t", value: THREE.ImageUtils.loadTextureCube(rainbowPaths) },
	};

    var material = new THREE.ShaderMaterial({
        uniforms: uniforms,
        vertexShader: document.getElementById('vertexShader').textContent,
        fragmentShader: document.getElementById('fragmentShader').textContent

    });
    
    cube = new THREE.Mesh(
        new THREE.BoxGeometry(20, 20, 20, 4, 4, 4),
        material);
    scene.add(cube);
        
    // Renderer
    renderer = new THREE.WebGLRenderer({ antialias: true, alpha: true });
    renderer.setPixelRatio(window.devicePixelRatio);
    renderer.setSize(width, height);
    container.appendChild(renderer.domElement);
    renderer.setClearColor(0x639FA7, 0);
    
    
    // Handle resizing
    window.addEventListener( 'resize', onWindowResize, false );
}

function onWindowResize() {
    getIt = document.getElementById('get-it');
    var width = getIt.clientWidth; 
    var height = 400;

    camera.aspect = width / height;
    camera.updateProjectionMatrix();
    
    renderer.setSize(width, height);
}

function animate() {
    requestAnimationFrame(animate);
    cube.rotation.set(cube.rotation.x + 0.001, cube.rotation.y + 0.002, cube.rotation.z + 0.001);
    camera.lookAt( scene.position );
    renderer.render(scene, camera);
}