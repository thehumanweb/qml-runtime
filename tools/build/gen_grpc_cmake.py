#!/usr/bin/env python

import mako.template
import os
import sys
import yaml

TOOLS_DIR = os.path.dirname(os.path.realpath(__file__))
ROOT_DIR = os.path.abspath(os.path.join(TOOLS_DIR, '..', '..'))
GRPC_DIR = os.path.abspath(os.path.join(ROOT_DIR, 'src', '3rdparty', 'grpc'))
GRPC_CMAKE_DIR = os.path.abspath(os.path.join(ROOT_DIR, 'src', '3rdparty', 'cmake', 'grpc'))

class GrpcTarget(object):
  def __init__(self, name, src, deps, build, secure):
    self.name = name
    self.src = src
    self.deps = deps
    self.build = build
    self.secure = secure

def createGrpcCmake():
  def find_in_json(arr, name):
    for elem in arr:
      if elem['name'] == name:
        return elem

  def collect_grpc_targets(acc, dic, name, executable=False):
    key = 'libs' if not executable else 'targets'
    if name in acc[key]:
      return
    target = find_in_json(dic[key], name)
    if not target:
      print('Not found "%s" in %s' % (name, key))
    src = target['src']
    for fg in target.get('filegroups', []):
      src.extend(find_in_json(dic['filegroups'], fg)['src'])
    deps = target.get('deps', [])
    secure = (target['secure'] == True)
    
    acc[key][name] = GrpcTarget(
        name, src, deps, target['build'], secure)
    for dep in deps:
      collect_grpc_targets(acc, dic, dep)

  buildyaml = os.path.join(GRPC_DIR, 'build.yaml')
  with open(buildyaml, 'r') as fp:
    dic = yaml.load(fp)
  acc = {
    'targets': {},
    'libs': {},
  }
  collect_grpc_targets(acc, dic, 'grpc++')
  # TODO enable cpp plugin
  # collect_grpc_targets(acc, dic, 'grpc_cpp_plugin', True)

  cmake_template = os.path.join(TOOLS_DIR, 'CMakeLists.txt.grpc.template')
  cmake_out = os.path.join(GRPC_CMAKE_DIR, 'CMakeLists.txt')
  with open(cmake_out, 'w+') as fp:
    fp.write(mako.template.Template(filename=cmake_template).render(**acc))


if __name__ == '__main__':
  createGrpcCmake()
  sys.exit(0)
