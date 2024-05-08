#!/usr/bin/env -S node

// Script to bundle RedFileSystem to dist/ and release it as an archive.
//
// Requirements:
//  - npm install --save-dev archiver
//
// Usage: node bundle.mjs
import * as fs from 'fs';
import archiver from 'archiver';

const PLUGIN_VERSION = '0.7.0';
const PLUGIN_NAME = 'RedFileSystem';
const PLUGIN_LICENSE = 'LICENSE';
const PLUGIN_LIBRARY_PATH = `build/Release/${PLUGIN_NAME}.dll`;
const PLUGIN_SCRIPT_PATH = `scripts/${PLUGIN_NAME}/`;

const BUNDLE_PATH = 'dist/';
const BUNDLE_LIBRARY_PATH = `${BUNDLE_PATH}red4ext/plugins/${PLUGIN_NAME}/`;
const BUNDLE_SCRIPT_PATH = `${BUNDLE_PATH}r6/scripts/${PLUGIN_NAME}/`;

/// Entry-point ///

console.log(`${PLUGIN_NAME} · v${PLUGIN_VERSION}`);
console.log('Bundling...');

// Create bundle structure.
fs.rmSync(BUNDLE_PATH, {recursive: true, force: true});
fs.mkdirSync(BUNDLE_LIBRARY_PATH, {recursive: true});
fs.mkdirSync(BUNDLE_SCRIPT_PATH, {recursive: true});

// Copy library.
try {
    fs.copyFileSync(PLUGIN_LIBRARY_PATH, `${BUNDLE_LIBRARY_PATH}/${PLUGIN_NAME}.dll`);
} catch (error) {
    console.log(`Plugin not found in "build/Release/${PLUGIN_NAME}.dll".`);
    console.log('Build library in release mode.');
    process.exit(1);
}

// Copy license.
fs.cpSync(PLUGIN_LICENSE, `${BUNDLE_SCRIPT_PATH}/${PLUGIN_LICENSE}`);

// Prepare modules.
const files = fs.readdirSync(PLUGIN_SCRIPT_PATH, {withFileTypes: true});
const scripts = files.filter(filterRedscript);
const modules = files.filter(filterDirectory);
const graph = {
    root: {
        name: '',
        path: PLUGIN_SCRIPT_PATH,
        scripts: scripts,
        imports: ['$RedData.Json.*']
    },
    modules: []
};

modules.forEach((module) => {
    if (module.name === 'Test') {
        return;
    }
    const path = `${module.path}${module.name}/`;
    const files = fs.readdirSync(path, {withFileTypes: true});
    const scripts = files.filter(filterRedscript);

    graph.modules.push({
        name: module.name,
        path: path,
        scripts: scripts,
        imports: []
    });
});

// Create modules.
let totalSize = createModule(graph.root, BUNDLE_SCRIPT_PATH);

for (const dependency of graph.modules) {
    totalSize += createModule(dependency, BUNDLE_SCRIPT_PATH);
}
console.log(`Total modules size: ${totalSize} bytes`);
console.log('');
console.log('Archiving...');

// Create an archive to release.
const archivePath = `${PLUGIN_NAME}-v${PLUGIN_VERSION}.zip`;
const archiveStream = fs.createWriteStream(archivePath);
const archive = archiver('zip', {zlib: {level: 9}});

archive.pipe(archiveStream);
archive.directory('dist/', false);
archive.on('error', (error) => {
    console.log('Failed to build archive :(');
    console.error(error);
});
archive.on('end', () => {
    console.log(`Archive ready ${archivePath} (${getSize(archivePath)} bytes)`);
    fs.rmSync(BUNDLE_PATH, {recursive: true, force: true});
});
archive.finalize();


/// Helpers ///

function createModule(module, path) {
    if (module.scripts.length === 0) {
        console.log(` · no entry-point module`);
        return 0;
    }
    let moduleName;

    if (module.name.length > 0) {
        moduleName = `${PLUGIN_NAME}.${module.name}`;
    } else {
        moduleName = `${PLUGIN_NAME}`;
    }
    path = `${path}${moduleName}.reds`;
    let data = `// ${PLUGIN_NAME} v${PLUGIN_VERSION}\n`;

    data += `module ${moduleName}\n`;
    for (const dependencyName of module.imports) {
        if (dependencyName.startsWith('$')) {
            data += `import ${dependencyName.slice(1)}\n`;
        } else {
            data += `import ${PLUGIN_NAME}.${dependencyName}.*\n`;
        }
    }
    data += '\n';
    for (const script of module.scripts) {
        data += fs.readFileSync(`${module.path}${script.name}`, {encoding: 'utf8'}).trim();
        data += '\n\n';
    }
    data = data.trimEnd();
    fs.writeFileSync(path, data);
    let size = getSize(path);

    console.log(` · module ${moduleName} (${size} bytes)`);
    return size;
}

function filterRedscript(file) {
    return file.isFile() && scriptRule(file.name);
}

function filterDirectory(file) {
    return file.isDirectory();
}

function getSize(path) {
    return fs.lstatSync(path).size;
}

function scriptRule(name) {
    return name.endsWith('.reds') && !name.endsWith('Test.reds');
}
