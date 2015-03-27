// Set up the scene, camera, and renderer as global variables.
var container;

var scene, camera, renderer;

var uniforms;

var cube;

var eyeVector;
var sceneRange = 20.0;

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

    eyeVector = new THREE.Vector3( -sceneRange, -sceneRange, -sceneRange);
                
    uniforms = {
        u_eyeVector: { type: 'v3', value: eyeVector},
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
    updateEyeVector();
    camera.lookAt( scene.position );
    renderer.render(scene, camera);
}

function updateEyeVector() {
    if (eyeVector.x + 0.1 > sceneRange)
        eyeVector.setX( -sceneRange );
    else
        eyeVector.setX( eyeVector.x + 0.01 );

    if (eyeVector.y + 0.2 > sceneRange)
        eyeVector.setY( -sceneRange );
    else
        eyeVector.setY( eyeVector.y + 0.02 );

    if (eyeVector.z + 0.3 > sceneRange)
        eyeVector.setZ( -sceneRange );
    else
        eyeVector.setZ( eyeVector.z + 0.03 );
}