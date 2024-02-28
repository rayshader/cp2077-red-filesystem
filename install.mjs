#!/usr/bin/env -S node

// Script to install RedFileSystem in your game's directory.
// Usage: node --experimental-modules install.mjs [game-path]
import * as fs from 'fs';

const options = process.argv.slice(2);

const GAME_PATH = options.length > 0 ? options[0] : 'C:\\Program Files (x86)\\Steam\\steamapps\\common\\Cyberpunk 2077';
const RED4EXT_PATH = `${GAME_PATH}\\red4ext\\plugins`;
const REDSCRIPT_PATH = `${GAME_PATH}\\r6\\scripts`;

// Whether game directory exists?
if (!fs.existsSync(GAME_PATH)) {
    console.error(`[RedFS] Could not find game directory at: ${GAME_PATH}`);
    process.exit(1);
}

// Whether RED4ext directory exists?
if (!fs.existsSync(RED4EXT_PATH)) {
    console.error(`[RedFS] Could not find RED4ext directory at: ${RED4EXT_PATH}`);
    console.info('[RedFS] Install RED4ext from https://docs.red4ext.com/getting-started/installing-red4ext');
    process.exit(2);
}

// Whether Redscript directory exists?
if (!fs.existsSync(REDSCRIPT_PATH)) {
    console.error(`[RedFS] Could not find Redscript directory at: ${REDSCRIPT_PATH}`);
    console.info('[RedFS] Install Redscript from https://wiki.redmodding.org/redscript/getting-started/downloads');
    process.exit(3);
}

const GAME_PLUGIN_PATH = `${RED4EXT_PATH}\\RedFileSystem`
const GAME_SCRIPT_PATH = `${REDSCRIPT_PATH}\\RedFileSystem`

// Optionally create plugin's directory.
if (!fs.existsSync(GAME_PLUGIN_PATH)) {
    fs.mkdirSync(GAME_PLUGIN_PATH);
    console.info(`[RedFS] Plugin directory created at: ${GAME_PLUGIN_PATH}`);
}

// Delete all scripts, to prevent old (removed) scripts to persist.
fs.rmSync(GAME_SCRIPT_PATH, {force: true, recursive: true});
fs.mkdirSync(GAME_SCRIPT_PATH);
console.info(`[RedFS] Script directory created at: ${GAME_SCRIPT_PATH}`);

const BUILD_LIBRARY_PATH = `build\\Debug\\RedFileSystem.dll`;
const GAME_LIBRARY_PATH = `${GAME_PLUGIN_PATH}\\RedFileSystem.dll`;

fs.cpSync(BUILD_LIBRARY_PATH, GAME_LIBRARY_PATH, {force: true});
console.info(`[RedFS] Library installed.`);

const BUILD_SCRIPT_PATH = `scripts\\RedFileSystem`;

fs.cpSync(BUILD_SCRIPT_PATH, GAME_SCRIPT_PATH, {force: true, recursive: true, preserveTimestamps: true});
console.info(`[RedFS] Scripts installed.`);