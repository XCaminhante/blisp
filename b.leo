<?xml version="1.0" encoding="utf-8"?>
<!-- Created by Leo: http://leoeditor.com/leo_toc.html -->
<leo_file xmlns:leo="http://leoeditor.com/namespaces/leo-python-editor/1.1" >
<leo_header file_format="2"/>
<globals/>
<preferences/>
<find_panel_settings/>
<vnodes>
<v t="caminhante.20200313155940.1"><vh>@settings</vh>
<v t="caminhante.20200619162742.1"><vh>@command open-url-here @key=Ctrl-U</vh></v>
<v t="caminhante.20200618180759.1"><vh>@command terminal-here @key=Alt-T</vh></v>
<v t="caminhante.20200313155940.99"><vh>nodeActions</vh>
<v t="caminhante.20200604232050.1"><vh>@cmd *</vh></v>
<v t="caminhante.20200619161501.1"><vh>@download *</vh></v>
<v t="caminhante.20200619155412.1"><vh>@duck *</vh></v>
<v t="caminhante.20200927204806.1"><vh>@gcc|@gcc .* [X]</vh></v>
<v t="caminhante.20200808174417.1"><vh>@js|@js .* [X]</vh></v>
<v t="caminhante.20200912185313.1"><vh>@lua|@lua .* [X]</vh></v>
<v t="caminhante.20210501142945.2"><vh>@mkdir *</vh></v>
<v t="caminhante.20210501142945.3"><vh>@mv file://*</vh></v>
<v t="caminhante.20200619213937.1"><vh>@node|@node .* [X]</vh></v>
<v t="caminhante.20210501142945.4"><vh>@rmdir /*/</vh></v>
<v t="caminhante.20200618203115.1"><vh>@run|@run .* [X]</vh></v>
<v t="caminhante.20200619154823.1"><vh>@web *</vh></v>
</v>
</v>
<v t="caminhante.20210508214940.1"><vh>B Lisp</vh>
<v t="caminhante.20210508215133.1"><vh>@path .</vh>
<v t="caminhante.20210508223123.3"><vh>.gitmodules</vh></v>
<v t="caminhante.20210508215135.4"><vh>/.git/</vh></v>
<v t="caminhante.20210508223123.2"><vh>/minimal_UTF8/</vh>
<v t="caminhante.20210508223133.7"><vh>.git</vh></v>
<v t="caminhante.20210508223133.2"><vh>@edit utf8.c</vh></v>
<v t="caminhante.20210508223133.1"><vh>@edit utf8.h</vh></v>
<v t="caminhante.20210508223133.6"><vh>LICENSE</vh></v>
<v t="caminhante.20210508223133.5"><vh>Makefile</vh></v>
<v t="caminhante.20210508223133.3"><vh>minimal_UTF8.leo</vh></v>
<v t="caminhante.20210508223133.4"><vh>README.md</vh></v>
</v>
<v t="caminhante.20210508215135.2"><vh>@auto README.md</vh></v>
<v t="caminhante.20210508220611.1"><vh>@edit .gitignore</vh></v>
<v t="caminhante.20210508215135.3"><vh>@edit LICENSE</vh></v>
<v t="caminhante.20210508220429.1"><vh>@edit Makefile</vh></v>
<v t="caminhante.20210508215346.1"><vh>@file blisp.c</vh></v>
<v t="caminhante.20210508215135.1"><vh>b.leo</vh></v>
</v>
</v>
</vnodes>
<tnodes>
<t tx="caminhante.20200313155940.1"></t>
<t tx="caminhante.20200313155940.99">@language python
</t>
<t tx="caminhante.20200604232050.1">import subprocess

def execute (cmd):
    return subprocess.run(cmd,shell=True,universal_newlines=True,stderr=subprocess.STDOUT,stdout=subprocess.PIPE).stdout

if c.isChanged(): c.save()

path = c.getNodePath(c.p)
g.os.chdir(path)

command = ''.join(c.p.h.split('@cmd ')[1:])
command = "cat &lt;&lt;'EOFEOFEOFEOFEOFEOF' | bash\n"+command+'\nEOFEOFEOFEOFEOFEOF'
g.es(c.p.h)
c.p.b += execute(command)

c.save()
</t>
<t tx="caminhante.20200618180759.1">@language python
import subprocess

def execute (cmd):
    return subprocess.run(cmd,shell=True,universal_newlines=True,stderr=subprocess.STDOUT,stdout=subprocess.PIPE).stdout

if c.isChanged(): c.save()
path = c.getNodePath(c.p)

execute('terminator --working-directory="'+path+'" --new-tab')
</t>
<t tx="caminhante.20200618203115.1">@language python
import subprocess

# def getpath (p):
    # dict = c.scanAllDirectives(p)
    # d = dict.get("path")
    # if p.isAnyAtFileNode():
        # filename = p.anyAtFileNodeName()
        # filename = g.os_path_join(d,filename)
        # if filename:
            # d = g.os_path_dirname(filename)
    # if d is None:
        # return ""
    # else:
        # return g.os_path_normpath(d)

# if c.isChanged(): c.save()

def execute (cmd):
    return subprocess.run(cmd,shell=True,universal_newlines=True,stderr=subprocess.STDOUT,stdout=subprocess.PIPE).stdout

path = c.getNodePath(c.p)
g.os.chdir(path)

w = c.frame.body.wrapper
s1,s2 = w.getSelectionRange()

if s1 == s2:
    code = c.p.b
else:
    linebr = c.p.b.find('\n',s2)
    if linebr == -1: linebr = len(c.p.b)
    code = c.p.b[s1:s2]

g.es(c.p.h)

command = "cat &lt;&lt;'EOFEOFEOFEOFEOFEOF' | bash\n"+code+'\nEOFEOFEOFEOFEOFEOF'

# if s1 == s2:
    # c.p.b += '\n' + execute(command)
# else:
    # c.p.b = c.p.b[0:linebr] + '\n' + execute(command) + c.p.b[linebr:]

cpc = c.p.copy()
novoNo = cpc.insertAsLastChild()
novoNo.h = c.p.h
novoNo.b = execute(command)
c.redraw()

c.save()
w.setSelectionRange(s1,s2)
</t>
<t tx="caminhante.20200619154823.1">import subprocess

def execute (cmd):
    return subprocess.run(cmd,shell=True,universal_newlines=True,stderr=subprocess.STDOUT,stdout=subprocess.PIPE).stdout

if c.isChanged(): c.save()

url = ''.join(c.p.h.split('@web ')[1:])
command = 'lynx -image_links -dump "'+url+'"'
g.es(c.p.h)
c.p.b = execute(command)

c.save()
</t>
<t tx="caminhante.20200619155412.1">import subprocess
import urllib.parse

def execute (cmd):
    return subprocess.run(cmd,shell=True,universal_newlines=True,stderr=subprocess.STDOUT,stdout=subprocess.PIPE).stdout

if c.isChanged(): c.save()

search = ''.join(c.p.h.split('@duck ')[1:])
searchenc = urllib.parse.quote(search)
command = 'lynx -dump "https://duckduckgo.com/html/?q='+searchenc+'&amp;t=seamonkey"'
g.es(c.p.h)
c.p.b = execute(command)

c.save()
</t>
<t tx="caminhante.20200619161501.1">import subprocess

def execute (cmd):
    return subprocess.run(cmd,shell=True,universal_newlines=True,stderr=subprocess.STDOUT,stdout=subprocess.PIPE).stdout

if c.isChanged(): c.save()

path = c.getNodePath(c.p)
g.os.chdir(path)

url = ''.join(c.p.h.split('@download ')[1:])
command = 'wget --no-config --continue --no-verbose --adjust-extension --tries=3 --content-on-error "'+url+'"'
g.es(c.p.h)
c.p.b += execute(command)

c.save()
</t>
<t tx="caminhante.20200619162742.1">@language python
# if c.isChanged(): c.save()

w = c.frame.body.wrapper
b = c.p.b
i,j = w.getSelectionRange()

selecao = b[i:j]

cpc = c.p.copy()
novoNo = cpc.insertAsLastChild()
c.setHeadString(novoNo, '@web '+selecao)
c.selectPosition(novoNo)
c.redraw()
</t>
<t tx="caminhante.20200619213937.1">import subprocess
import re

def execute (cmd):
    return subprocess.run(cmd,shell=True,universal_newlines=True,stderr=subprocess.STDOUT,stdout=subprocess.PIPE).stdout

path = c.getNodePath(c.p)
g.os.chdir(path)

code = re.sub(r'^#|\n+#', '\n///', g.getScript(c,c.p))

g.es(c.p.h)

command = "cat &lt;&lt;'EOFEOFEOFEOFEOFEOF' | node\n"+code+'\nEOFEOFEOFEOFEOFEOF'

c.p.b += '\n' + execute(command)
</t>
<t tx="caminhante.20200808174417.1">import subprocess
import re

def execute (cmd):
    return subprocess.run(cmd,shell=True,universal_newlines=True,stderr=subprocess.STDOUT,stdout=subprocess.PIPE).stdout

path = c.getNodePath(c.p)
g.os.chdir(path)

code = re.sub(r'^#|\n+#', '\n///', g.getScript(c,c.p))

g.es(c.p.h)

command = "cat &lt;&lt;'EOFEOFEOFEOFEOFEOF' | js\n"+code+'\nEOFEOFEOFEOFEOFEOF'

c.p.b += '\n' + execute(command)
</t>
<t tx="caminhante.20200912185313.1">import subprocess
import re

def execute (cmd):
    return subprocess.run(cmd,shell=True,universal_newlines=True,stderr=subprocess.STDOUT,stdout=subprocess.PIPE).stdout

path = c.getNodePath(c.p)
g.os.chdir(path)

code = re.sub(r'^#|\n+#', '\n---', g.getScript(c,c.p))

g.es(c.p.h)

command = "cat &lt;&lt;'EOFEOFEOFEOFEOFEOF' | lua\n"+code+'\nEOFEOFEOFEOFEOFEOF'

c.p.b += '\n' + execute(command)
</t>
<t tx="caminhante.20200927204806.1">import subprocess
import re

def execute (cmd):
    return subprocess.run(cmd,shell=True,universal_newlines=True,stderr=subprocess.STDOUT,stdout=subprocess.PIPE).stdout

path = c.getNodePath(c.p)
g.os.chdir(path)

code = re.sub(r'^#@|\n+#@', '\n///', g.getScript(c,c.p))
args = ''.join(c.p.h.split('@gcc ')[1:])

g.es(c.p.h)

command = "cat &lt;&lt;'EOFEOFEOFEOFEOFEOF' | gcc -xc - -Wall -Werror -std=gnu11 -c -o /dev/null \n"+code+'\nEOFEOFEOFEOFEOFEOF'

c.p.b += '\n' + execute(command)
# g.es(command)
</t>
<t tx="caminhante.20210501142945.2">import subprocess

def execute (cmd):
    return subprocess.run(cmd,shell=True,universal_newlines=True,stderr=subprocess.STDOUT,stdout=subprocess.PIPE).stdout

path = c.getNodePath(c.p)
g.os.chdir(path)

path = ''.join(c.p.h.split('@mkdir ')[1:])
cmd = 'mkdir -p "' + path + '"'
g.es(c.p.h)
execute(cmd)
c.p.h = '/'+path+'/'
c.p.b = '@path ' + path
</t>
<t tx="caminhante.20210501142945.3">import subprocess

def execute (cmd):
    return subprocess.run(cmd,shell=True,universal_newlines=True,stderr=subprocess.STDOUT,stdout=subprocess.PIPE).stdout

path = c.getNodePath(c.p)
g.os.chdir(path)

path = ''.join(c.p.h.split('@mv file://')[1:])
cmd = 'mv -v "' + path + '" ./'
g.es(c.p.h)
g.es(execute(cmd))
</t>
<t tx="caminhante.20210501142945.4">import subprocess
import shutil

def execute (cmd):
    return subprocess.run(cmd,shell=True,universal_newlines=True,stderr=subprocess.STDOUT,stdout=subprocess.PIPE).stdout

ps = c.p.parents()
for par in ps:
    break
path = c.getNodePath(par)
g.os.chdir(path)

path = ''.join(c.p.h.split('@rmdir /')[1:]).split('/')[0]
shutil.rmtree(path)
</t>
<t tx="caminhante.20210508214940.1">B</t>
<t tx="caminhante.20210508215133.1"></t>
<t tx="caminhante.20210508215135.1"></t>
<t tx="caminhante.20210508215135.4">@path .git</t>
<t tx="caminhante.20210508223123.2">@path minimal_UTF8</t>
<t tx="caminhante.20210508223123.3"></t>
<t tx="caminhante.20210508223133.3"></t>
<t tx="caminhante.20210508223133.4"></t>
<t tx="caminhante.20210508223133.5"></t>
<t tx="caminhante.20210508223133.6"></t>
<t tx="caminhante.20210508223133.7"></t>
</tnodes>
</leo_file>
