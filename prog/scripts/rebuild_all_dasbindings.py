import os
import subprocess
import sys

bindings_checks = [
  {"wdir":"../../skyquake/prog","cmds":
    ("jam  -sPlatform=win64 -sRoot=../.. -f aot/jamfile",)
  },
  {"wdir":"../../skyquake/prog/scripts","cmds":
    ("genDasevents.bat",)
  },
  {"wdir":"../../enlisted/prog","cmds":
    ("jam  -sPlatform=win64 -sRoot=../.. -f aot/jamfile", "genDasevents.bat")
  },
  {"wdir":"../../active_matter/prog","cmds":
    ("jam  -sPlatform=win64 -sRoot=../.. -f aot/jamfile", "genDasevents.bat")
  },
  {"wdir":"../../cuisine_royale/prog","cmds":
    ("jam  -sPlatform=win64 -sRoot=../.. -f aot/jamfile", "genDasevents.bat")
  },
  {"wdir":"../../modern_conflict/prog","cmds":
    ("jam  -sPlatform=win64 -sRoot=../.. -f aot/jamfile", "genDasevents.bat")
  },
  {"wdir":"../../to_the_sky/prog","cmds":
    ("jam  -sPlatform=win64 -sRoot=../.. -f aot/jamfile", "genDasevents.bat")
  },
]

class pushd:
    def __init__(self, path):
        print("changing wdir to:", path)
        self.olddir = os.getcwd()
        if path != '':
            os.chdir(os.path.normpath(path))
    def __enter__(self):
        pass
    def __exit__(self, type, value, traceback):
        os.chdir(self.olddir)

def shell(cmd, print_stdout=True):
    print("cmd:", cmd)
    res = []
    with subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, bufsize=1, universal_newlines=True) as p:
        for line in p.stdout:
            if print_stdout:
              print(line, end='') # process line here
            res.append(line)
    if p.returncode != 0:
        raise subprocess.CalledProcessError(p.returncode, p.args)
    return res

def main():
    for e in bindings_checks:
        wdir = e["wdir"]
        with pushd(wdir):
            for cmd in e["cmds"]:
                shell(cmd)

def are_files_changed_after_format():
  ret = shell('git status -s', print_stdout=False)
  res = []
  for i in ret:
    i = i.strip()
    if i.startswith("M "):
      res.append(i[2:])
  return res

if __name__ == "__main__":
  if "check_modified" in sys.argv:
    if files := are_files_changed_after_format():
      print("files modified:")
      for f in files:
        print(f)
      print("Failed test!")
      sys.exit(1)
  else:
    main()