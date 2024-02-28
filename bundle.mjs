#!/usr/bin/env -S node

// Script to bundle RedFileSystem to dist/ and release it as an archive.
// Usage: node --experimental-modules bundle.mjs
import * as fs from 'fs';

const PLUGIN_NAME = 'RedFileSystem';
const PLUGIN_LICENSE = 'LICENSE';
const PLUGIN_LIBRARY_PATH = `build/Release/${PLUGIN_NAME}.dll`;
const PLUGIN_SCRIPT_PATH = `scripts/${PLUGIN_NAME}/`;

const BUNDLE_PATH = 'dist/';
const BUNDLE_LIBRARY_PATH = `${BUNDLE_PATH}red4ext/plugins/${PLUGIN_NAME}/`;
const BUNDLE_SCRIPT_PATH = `${BUNDLE_PATH}r6/scripts/${PLUGIN_NAME}/`;

// Create bundle structure.
fs.rmSync(BUNDLE_PATH, {recursive: true, force: true});
fs.mkdirSync(BUNDLE_LIBRARY_PATH, {recursive: true});
fs.mkdirSync(BUNDLE_SCRIPT_PATH, {recursive: true});

// Copy library.
try {
    fs.copyFileSync(PLUGIN_LIBRARY_PATH, `${BUNDLE_LIBRARY_PATH}/${PLUGIN_NAME}.dll`);
} catch (error) {
    console.log('Plugin not found in "build/Release/RedFileSystem.dll".');
    console.log('Build library in release mode.');
    console.log(error);
    process.exit(1);
}

// Copy license.
fs.cpSync(PLUGIN_LICENSE, `${BUNDLE_SCRIPT_PATH}/${PLUGIN_LICENSE}`);

// Create modules.
const files = fs.readdirSync(PLUGIN_SCRIPT_PATH, {withFileTypes: true});
const scripts = files.filter(filterRedscript);
const modules = files.filter(filterDirectory);
const graph = {
    root: {
        name: '',
        path: PLUGIN_SCRIPT_PATH,
        scripts: scripts,
        imports: ['Enums', 'Json']
    },
    modules: []
};

modules.forEach((module) => {
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

createModule(graph.root, BUNDLE_SCRIPT_PATH);
for (const dependency of graph.modules) {
    createModule(dependency, BUNDLE_SCRIPT_PATH);
}

function createModule(module, path) {
    let moduleName;

    if (module.name.length > 0) {
        moduleName = `${PLUGIN_NAME}.${module.name}`;
    } else {
        moduleName = `${PLUGIN_NAME}`;
    }
    path = `${path}${moduleName}.reds`;
    let data = `module ${moduleName}`;

    for (const dep of module.imports) {
        data += `\nimport ${PLUGIN_NAME}.${dep}.*`;
    }
    for (const script of module.scripts) {
        data += '\n\n';
        data += fs.readFileSync(`${module.path}${script.name}`, {encoding: 'utf8'}).trim();
    }
    fs.writeFileSync(path, data);
    console.log(`Module "${moduleName}": "${path}"`);
}

function filterRedscript(file) {
    return file.isFile() && scriptRule(file.name);
}

function filterDirectory(file) {
    return file.isDirectory();
}

function scriptRule(name) {
    return name.endsWith('.reds') && !name.endsWith('Test.reds');
}
