// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
import { spawnSync } from 'node:child_process'
import { fileURLToPath } from 'node:url'
import path from 'node:path'
const web = fileURLToPath(new URL('..', import.meta.url))
const repo = path.dirname(web)
const build = path.join(web, 'build-conan-wasm')
for (const [command, args] of [
  [
    'conan',
    [
      'install',
      repo,
      '--output-folder',
      build,
      '--build=missing',
      '--lockfile',
      path.join(web, 'conan.lock'),
      '-pr:b',
      'default',
      '-pr:h',
      path.join(repo, 'conan-profiles/profiles/TransceiverTool-wasm'),
    ],
  ],
  [
    'cmake',
    [
      '-S',
      repo,
      '-B',
      build,
      `-DCMAKE_TOOLCHAIN_FILE=${build}/conan_toolchain.cmake`,
      '-DCMAKE_BUILD_TYPE=Release',
      '-DTRANSCEIVERTOOL_BUILD_CLI=OFF',
      '-DTRANSCEIVERTOOL_BUILD_WEB=ON',
    ],
  ],
  ['cmake', ['--build', build, '--target', 'transceiver_web', '--parallel', '4']],
]) {
  const result = spawnSync(command, args, { stdio: 'inherit' })
  if (result.error) console.error(result.error.message)
  if (result.status !== 0) process.exit(result.status || 1)
}
