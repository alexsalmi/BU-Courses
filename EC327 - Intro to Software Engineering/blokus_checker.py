"""checker for  blokus

Note: the author emails are not validated against the actual student list.

"""
VERSION = (1,6)
from collections import OrderedDict

splitter = '// MAIN. Do not change the below.'

Tests={}
elementval=OrderedDict([
('astyle',1.7),
('cpplint',5),
('basic_create_show',10),
('board',5),
('reset',5),
('rotate',5),
('flipud',5),
('fliplr',5),
('reset',5),
('play',10),
('show_tiles',5),
('resize_bigger',5),
('upperleft',5)])




elementval.update([('duplicate',15),
('disconnected',10),
('badtilebox',3.3),
('resize_smaller',5)])


Tests['duplicate']=[("""// Create 21 blokus tiles and some duplicates
create 1 *
create 2 ** ..
create 3 *** ... ...
create 3 **. *.. ...
create 4
****
....
....
....
create 4
**..
**..
....
....
create 4
***.
*...
....
....
create 4
**..
.**.
....
....
create 4
***.
.*..
....
....
create 5
*****
.....
.....
.....
.....
create 5
****.
...*.
.....
.....
.....
create 5
****.
..*..
.....
.....
.....
create 5
***..
..**.
.....
.....
.....
create 5
***..
.**..
.....
.....
.....
create 5
***..
*.*..
.....
.....
.....
create 5
.....
..*..
.***.
..*..
.....
create 5
..*..
.***.
.*...
.....
.....
create 5
.....
.*...
.*...
.***.
.....
create 5
...*.
.***.
.*...
.....
.....
create 5
...*.
.***.
...*.
.....
.....
create 5
...*.
..**.
.**..
.....
.....
create 6
......
......
.**...
..*...
..**..
......
create 6
......
..*...
..*...
.***..
......
......
create 6
......
..***.
....**
......
......
......
create 6
......
..**..
...**.
....*.
......
......
create 6
......
...*..
...*..
...*..
...*..
...*..
create 6
......
......
.***..
.*....
......
......
create 6
......
......
...**.
...**.
......
......
create 6
......
......
..**..
***...
......
......
create 6
......
******
......
......
......
......
quit
""","""created tile 100
created tile 101
created tile 102
created tile 103
created tile 104
created tile 105
created tile 106
created tile 107
created tile 108
created tile 109
created tile 110
created tile 111
created tile 112
created tile 113
created tile 114
created tile 115
created tile 116
created tile 117
created tile 118
created tile 119
created tile 120
duplicate of 118 discarded
duplicate of 119 discarded
duplicate of 112 discarded
duplicate of 120 discarded
duplicate of 109 discarded
duplicate of 106 discarded
duplicate of 105 discarded
duplicate of 112 discarded
created tile 121
Goodbye
"""),("""create 4
**..
***.
..**
...*
create 4
.***
**..
.***
.*..
create 6
******
*....*
*....*
****.*
*....*
******
create 6
******
*....*
****.*
*....*
*....*
******
create 6
******
*....*
*.****
*....*
*....*
******
create 1 *
create 4
..**
..**
...*
....
create 3
...
***
**.
create 5
.....
.....
.....
.....
....*
create 10
..........
****..*..*
*..*..*..*
*..*..*..*
***...*..*
*..*..*..*
*..*..*..*
*..*..*..*
**********
..........
rotate 105
rotate 105
create 2 ** **
create 10
********..
.......*..
.......*..
********..
.......*..
.......*..
***.****..
*..*...*..
*..*...*..
********..
create 10
**********
*..*..*..*
*..*..*..*
*..*..*..*
*..*...***
*..*..*..*
*..*..*..*
*..*..****
..........
..........
quit
""","""created tile 100
created tile 101
created tile 102
duplicate of 102 discarded
duplicate of 102 discarded
created tile 103
created tile 104
duplicate of 104 discarded
duplicate of 103 discarded
created tile 105
rotated 105
********..
.......*..
.......*..
********..
.......*..
.......*..
***.****..
*..*...*..
*..*...*..
********..
rotated 105
**********
*..*..*..*
*..*..*..*
*..*..*..*
*..*...***
*..*..*..*
*..*..*..*
*..*..****
..........
..........
created tile 106
duplicate of 105 discarded
duplicate of 105 discarded
Goodbye
"""),("""create 4
**.*
.***
***.
*.**
create 4
.***
**.*
***.
*.**
show 100
show 101
quit
""","""created tile 100
created tile 101
**.*
.***
***.
*.**
.***
**.*
***.
*.**
Goodbye
""")]

Tests['resize_smaller']=[("""// resize smaller and larger ,with tiles
resize 2
resize 5
resize 8
resize 2
resize 8
create 3 *** ..* ...
play 100 5 5
create 2 ** **
play 101 6 0
create 2 .* .*
play 102 1 6
create 4
****
.*..
....
....
play 103 4 3
create 4
****
*..*
*..*
***.
play 104 0 0
board
resize 7
resize 6
resize 3
quit
""","""..
..
.....
.....
.....
.....
.....
........
........
........
........
........
........
........
........
..
..
........
........
........
........
........
........
........
........
created tile 100
played 100
created tile 101
played 101
created tile 102
played 102
created tile 103
played 103
created tile 104
played 104
****....
*..*..*.
*..*..*.
***.....
...****.
....****
**.....*
**......
****...
*..*..*
*..*..*
***....
...****
....*..
.......
****..
*..*..
*..*..
***...
......
......
...
...
...
Goodbye
"""),("""// full resize test
// resize smaller and larger ,with tiles
resize 2
resize 5
resize 8
resize 2
resize 8
create 3 *** ..* ...
play 100 5 5
create 2 ** **
play 101 6 0
create 2 .* .*
play 102 1 6
create 4
****
.*..
....
....
play 103 4 3
board
resize 7
play 101 5 5
play 101 0 0
resize 6
play 101 2 0
play 101 0 2
resize 3
play 101 2 2
quit
""","""..
..
.....
.....
.....
.....
.....
........
........
........
........
........
........
........
........
..
..
........
........
........
........
........
........
........
........
created tile 100
played 100
created tile 101
played 101
created tile 102
played 102
created tile 103
played 103
........
......*.
......*.
........
...****.
....****
**.....*
**......
.......
......*
......*
.......
...****
....*..
.......
played 101
played 101
**....
**....
......
......
......
......
played 101
played 101
**.
**.
...
101 not played
Goodbye
""")]

scoring_elements=[
'basic_create_show','rotate','flipud',"fliplr",'board','reset','play',
'show_tiles','resize_bigger','upperleft',
'duplicate','disconnected','badtilebox','resize_smaller']

Tests['badtilebox']=[("""create 1 @@
create 2 ** **
create 2
xx
oo
show tiles
quit
""",
"""invalid tile
created tile 100
invalid tile
tile inventory
100
**
**
Goodbye
"""),
("""create 3
...
***
?..
create 1 q
create 2
**
*q
create 1 *
show tiles
quit
""","""invalid tile
invalid tile
invalid tile
created tile 100
tile inventory
100
*
Goodbye
""")]

Tests['disconnected']=[("""create 2 .* *.
create 2 *. .*
create 3
**.
..*
***
create 5
..*..
.**..
..*..
.*.*.
.***.
create 11
**********.
.........**
.*******..*
.*.....*..*
**........*
*..********
.......*...
.......*...
.*******...
.*......***
.********..
quit
""","""disconnected tile discarded
disconnected tile discarded
disconnected tile discarded
disconnected tile discarded
disconnected tile discarded
Goodbye
"""),("""// Disconnected tiles inside tileboxes
create 2 ** ..
create 2 .* *.
create 2 *. .*
create 3
**.
..*
***
create 1 *
show 100
show 101
quit
""","""created tile 100
disconnected tile discarded
disconnected tile discarded
disconnected tile discarded
created tile 101
**
..
*
Goodbye
""")]

Tests['upperleft']=[("""create 3 ... ..* ***
show 100
rotate 100
fliplr 100
rotate 100
flipud 100
quit
""",
"""created tile 100
..*
***
...
rotated 100
**.
.*.
.*.
fliplr 100
**.
*..
*..
rotated 100
*..
***
...
flipud 100
***
*..
...
Goodbye
""")]

Tests['resize_bigger']=[("""resize 2
resize 4
resize 6
create 2 ** **
play 100 4 4
board
resize 8
create 1 *
play 101 7 7
board
quit
""","""..
..
....
....
....
....
......
......
......
......
......
......
created tile 100
played 100
......
......
......
......
....**
....**
........
........
........
........
....**..
....**..
........
........
created tile 101
played 101
........
........
........
........
....**..
....**..
........
.......*
Goodbye
""")]
Tests['reset']=[("""create 2
**
**
resize 3
play 100 0 0
create 1 *
play 101 2 2
board
reset
show tiles
resize 3
quit
""","""created tile 100
...
...
...
played 100
created tile 101
played 101
**.
**.
..*
game reset
tile inventory
...
...
...
Goodbye
""")]


Tests['basic_create_show']=[
("""// Create tiles and show them 
// No duplicates, disconnects, or translations required
create 2 ** .*
create 3 *** *.* ***
create 2 ** **
create 4
*...
****
...*
****
create 5
...*.
*****
.....
.....
.....
show 100
show 101
show 104
show 103
show 102
quit
""",
"""created tile 100
created tile 101
created tile 102
created tile 103
created tile 104
**
.*
***
*.*
***
...*.
*****
.....
.....
.....
*...
****
...*
****
**
**
Goodbye
""")]

Tests['board']=[("""// board testing
board
resize 5
create 2 ** **
play 100 2 0
board
play 100 2 2
board
quit
""",
""".....
.....
.....
.....
.....
created tile 100
played 100
.....
.....
**...
**...
.....
played 100
.....
.....
****.
****.
.....
Goodbye
""")]


Tests['rotate']=[
("""create 5
.***.
.**..
*****
*.**.
..*..
rotate 100
rotate 100
rotate 100
rotate 100
create 3
.**
**.
.*.
rotate 101
rotate 101
rotate 101
rotate 101
quit
""","""created tile 100
rotated 100
..*..
*.**.
*****
***..
..**.
rotated 100
..*..
.**.*
*****
..**.
.***.
rotated 100
.**..
..***
*****
.**.*
..*..
rotated 100
.***.
.**..
*****
*.**.
..*..
created tile 101
rotated 101
*..
***
.*.
rotated 101
.*.
.**
**.
rotated 101
.*.
***
..*
rotated 101
.**
**.
.*.
Goodbye
""")]

Tests['flipud']=[("""create 5
.***.
.**..
*****
*.**.
..*..
flipud 100
flipud 100
create 3
.**
**.
.*.
flipud 101
flipud 101
create 3
***
..*
.**
flipud 102
flipud 102
quit
""","""created tile 100
flipud 100
..*..
*.**.
*****
.**..
.***.
flipud 100
.***.
.**..
*****
*.**.
..*..
created tile 101
flipud 101
.*.
**.
.**
flipud 101
.**
**.
.*.
created tile 102
flipud 102
.**
..*
***
flipud 102
***
..*
.**
Goodbye
""")]

Tests['fliplr']=[("""create 5
.***.
.**..
*****
*.**.
..*..
fliplr 100
fliplr 100
create 3
.**
**.
.*.
fliplr 101
fliplr 101
create 3
***
..*
.**
fliplr 102
fliplr 102
quit
""","""created tile 100
fliplr 100
.***.
..**.
*****
.**.*
..*..
fliplr 100
.***.
.**..
*****
*.**.
..*..
created tile 101
fliplr 101
**.
.**
.*.
fliplr 101
.**
**.
.*.
created tile 102
fliplr 102
***
*..
**.
fliplr 102
***
..*
.**
Goodbye
""")]


Tests['play']=[("""resize 4
create 3 *** ... ...
play 100 1 0
play 100 2 1
play 100 3 2
board
quit
""","""....
....
....
....
created tile 100
played 100
played 100
100 not played
....
***.
.***
....
Goodbye
"""),("""resize 3
create 2 ** *.
play 100 0 0
play 100 1 1
board
play 100 2 2
play 100 2 0
play 100 0 2
create 1 *
play 101 2 0
play 101 0 2
play 101 2 2
board
quit
""","""...
...
...
created tile 100
played 100
played 100
**.
***
.*.
100 not played
100 not played
100 not played
created tile 101
played 101
played 101
played 101
***
***
***
Goodbye
""")]

Tests['show_tiles']=[("""create 1 *
create 2 ** ..
create 3 *** ... ...
create 2 ** *.
create 4
****
....
....
....
create 2
**
**
create 3
***
*..
...
create 3
**.
.**
...
create 3
***
.*.
...
create 5
*****
.....
.....
.....
.....
create 4
****
...*
....
....
create 4
****
..*.
....
....
create 4
***.
..**
....
....
create 3
***
.**
...
create 3
***
*.*
...
create 3
.*.
***
.*.
create 3
.*.
***
*..
create 3
*..
*..
***
create 3
..*
***
*..
create 3
..*
***
..*
create 3
..*
.**
**.
show tiles
quit
""","""created tile 100
created tile 101
created tile 102
created tile 103
created tile 104
created tile 105
created tile 106
created tile 107
created tile 108
created tile 109
created tile 110
created tile 111
created tile 112
created tile 113
created tile 114
created tile 115
created tile 116
created tile 117
created tile 118
created tile 119
created tile 120
tile inventory
120
..*
.**
**.
119
..*
***
..*
118
..*
***
*..
117
*..
*..
***
116
.*.
***
*..
115
.*.
***
.*.
114
***
*.*
...
113
.**
***
...
112
***.
..**
....
....
111
..*.
****
....
....
110
****
...*
....
....
103
**
*.
102
***
...
...
101
**
..
100
*
104
****
....
....
....
105
**
**
106
***
*..
...
107
.**
**.
...
108
***
.*.
...
109
*****
.....
.....
.....
.....
Goodbye
""")]


DEBUG = False

try:
    from subprocess import PIPE,Popen,run,TimeoutExpired
    import re
    import sys
    import cpplint
    import difflib
    from io import StringIO
    import time
    import hashlib
    import itertools
    import logging
    import multiprocessing
    import numpy
    import string
except Exception as e:
    print('You are missing a part of the anaconda python3 distibution')
    print('This is required for running the checker.')
    print(e)
    quit()

CPPLINT_IGNORE = ['readability/alt_tokens','build/include_subdir']

MAXAUTHORS = 3

MAXSIZE = 20000

NOBRACKETS = True

AUTHWARN = "WARNING, NO VALID AUTHOR LINES FOUND"


ASTYLE_OPTIONS = [
    '--style=google', '--indent=spaces=2', '--formatted', '--dry-run'
]

REQUIRED_INCLUDES = ['algorithm','iomanip','iostream','string','vector']

VALID_INCLUDES = REQUIRED_INCLUDES + ['utility','array','tuple','map','algorithm',"set","unordered_map","unordered_set"]



real_main=splitter + r"""

int main() {
  string command;
  Blokus b;

  while (true) {
    cin >> command;
    if (command == "quit")  {
      break;
    } else if (command == "//") {
      getline(cin, command);
    } else if (command == "board") {
      b.show_board();
    } else if (command == "create") {
      b.create_piece();
    } else if (command == "reset") {
      b.reset();
    } else if (command == "show") {
      string arg;
      cin >> arg;
      if (arg == "tiles") {
        b.show_tiles();
      } else {
        auto g = b.find_tile(std::stoi(arg));
        g->show();
      }
    } else if (command == "resize") {
      int newsize;
      cin >> newsize;
      b.set_size(newsize);
      b.show_board();
    } else if (command == "play") {
      TileID id;
      int row, col;
      cin >> id >> row >> col;
      b.play_tile(id, row, col);
    } else if (command == "rotate") {
      TileID id;
      cin >> id;
      auto g = b.find_tile(id);
      g->rotate();
      cout << "rotated " << id << "\n";
      g->show();
    } else if (command == "fliplr") {
      TileID id;
      cin >> id;
      auto g = b.find_tile(id);
      g->fliplr();
      cout << "fliplr " << id << "\n";
      g->show();
    } else if (command == "flipud") {
      TileID id;
      cin >> id;
      auto g = b.find_tile(id);
      g->flipud();
      cout << "flipud " << id << "\n";
      g->show();
    } else {
      cout << "command not understood.\n";
    }
  }
  cout << "Goodbye\n";
  return 0;
}
"""

class TimeoutException(Exception):
    pass


class RunableProcessing(multiprocessing.Process):
    def __init__(self, func, *args, **kwargs):
        self.queue = multiprocessing.Queue(maxsize=1)
        args = (func,) + args
        multiprocessing.Process.__init__(self, target=self.run_func, args=args, kwargs=kwargs)

    def run_func(self, func, *args, **kwargs):
        try:
            result = func(*args, **kwargs)
            self.queue.put((True, result))
        except Exception as e:
            self.queue.put((False, e))

    def done(self):
        return self.queue.full()

    def result(self):
        return self.queue.get()


def timeout(seconds, force_kill=True):
    def wrapper(function):
        def inner(*args, **kwargs):
            now = time.time()
            proc = RunableProcessing(function, *args, **kwargs)
            proc.start()
            proc.join(seconds)
            if proc.is_alive():
                if force_kill:
                    proc.terminate()
                runtime = int(time.time() - now)
                raise TimeoutException('timed out after {0} seconds'.format(runtime))
            assert proc.done()
            success, result = proc.result()
            if success:
                return result
            else:
                raise result
        return inner
    return wrapper

COMMENT_STRING = {'py': '#', 'sh': "#", 'cpp': '//'}



def read_file(filename):
    "read the contents of filename into string"
    filehand = open(filename)
    contents = filehand.read()
    filehand.close()
    return contents


def get_includes(file_contents):
    "get included libraries in C/C++"
    includes = set()
    for line in file_contents.lower().splitlines():
        text = line.strip()
        search_str = r"#include\s*<(.*)>"
        matches = re.match(search_str, text)
        if matches:
            includes.add(matches.group(1))
        matches = re.match("#include \"(.*)\"", text)
        if matches:
            includes.add(matches.group(1))
    return includes

tiny_main="""int main() {
    return 0;
}
"""

def read_file_for_cpplint(filename):
    "read the contents of filename into list of strings"
    filehand = open(filename)
    contents = filehand.read()
    filehand.close()
    first_part,second_part=contents.split(splitter)    
    contents = first_part+tiny_main
    lines = contents.splitlines()
    if contents.endswith('\n'):
        lines.append('')
    return lines


def code_analysis_cpp(program_filename):
    Errors = {}
    def error_fcn(filename,line_number,lint_type,level,message):
        if lint_type not in CPPLINT_IGNORE:
            category,subcategory = lint_type.split('/')
            if category not in Errors:
                Errors[category]=[]
            Errors[category].append( (line_number,lint_type,message) )

    lines = read_file_for_cpplint(program_filename)
    cpplint.RemoveMultiLineComments(program_filename,lines,error_fcn)

    clean_lines = cpplint.CleansedLines(lines)

    cpplint.ProcessFileData(program_filename,'cpp',lines,error_fcn)

    num_lines = sum(bool(x.strip()) for x in clean_lines.lines)
    num_words = sum(len(x.split()) for x in clean_lines.lines)
    num_chars = sum(len(x) for x in clean_lines.lines)

    original = read_file(program_filename)
    proc_astyle = run(
        ['astyle', *ASTYLE_OPTIONS],
        input=original.encode(),
        stdout=PIPE,
        stderr=PIPE)

    if proc_astyle.returncode:
        unchanged='error'
    else:
        original = original.splitlines()
        newprog = proc_astyle.stdout.decode().splitlines()
        matcher = difflib.SequenceMatcher()
        matcher.set_seqs(original, newprog)
        unchanged = matcher.ratio()



    return {'chars':num_chars, 'lines': num_lines, 'words': num_words, 'errors':Errors,'astyle':unchanged}



SIZE_REPORT_TEMPLATE = """chars in code    : {}
lines of code    : {}, {:4.0%} of reference
tokens in code   : {}, {:4.0%} of reference"""


def code_size_report(submitted_code, reference_code):
    "generate message about code size"
    return SIZE_REPORT_TEMPLATE.format(
        submitted_code['chars'],
        submitted_code['lines'],
        submitted_code['lines'] / reference_code['lines'],
        submitted_code['words'],
        submitted_code['words'] / reference_code['words'])


def get_authors(file_contents, ptype):
    """get the authors in file_contents"""
    authors = []
    if ptype == 'json':
        A = json.loads(file_contents)
        return A.get('authors',[])

    for line in file_contents.lower().splitlines():
        if line.startswith(COMMENT_STRING[ptype]) and "copyright" in line:
            try:
                _, email = line.strip().rsplit(" ", 1)
                if email.endswith('@bu.edu'):
                    authors.append(email)
            except:
                pass
    return authors

popen_specs={'stdout':PIPE,'stdin':PIPE,'stderr':PIPE,'universal_newlines':True}

run_specs={'stdout':PIPE,'stderr':PIPE,'universal_newlines':True,'timeout':0.5}


#@timeout(15)
def blokus_tester(program_to_run):

    res = ""
    score = {}

    for scoring_element in sorted(scoring_elements):
        correct = 0
        incorrect = 0
        res += 'testing {}\n'.format(scoring_element)
        print(scoring_element,len(Tests[scoring_element]))
        for instring,answer in Tests[scoring_element]:
            val = ""
            try:
              T=run([program_to_run],input=instring,**run_specs)
            except TimeoutExpired:
              val = "timeout on {} test\n".format(scoring_element)

            if T.returncode:
              val += "Your program exited with return code {}\n".format(T.returncode)
            if T.stderr:
              val += "Your progam produced error messages:\n{}\n".format(T.stderr)
            if DEBUG:
              print('testing',scoring_element)
            if scoring_element == "show_tiles":
              my_text=answer.splitlines()
              your_text=T.stdout.splitlines()
              my_text.sort()
              your_text.sort()
              if my_text != your_text:
                  val += 'your answer differed from my answer, as follows:\n'
                  val += "your lines: {}\n  my lines: {}\n".format(len(your_text),len(my_text))
                  val += "{:20} {:20} {}\n".format("MY TEXT","YOUR TEXT","DIFFERENT")
                  for my,your in zip(my_text,your_text):
                     if my != your:
                         val +="{:20} {:20} {}\n".format(my,your,"<---")
                  val += "\n\n"  
            elif answer != T.stdout:
              val += 'your answer differed from my answer, as follows:\n'
              my_text=answer.splitlines()
              your_text=T.stdout.splitlines()
              val += "your lines: {}\n  my lines: {}\n".format(len(your_text),len(my_text))
              val += "{:20} {:20} {}\n".format("MY TEXT","YOUR TEXT","DIFFERENT")
              for my,your in zip(my_text,your_text):
                 val +="{:20} {:20} {}\n".format(my,your,"<---" if my != your else "")
            if scoring_element=="duplicate":
                print(answer)
                print(T.stdout)
            if not val:
                correct += 1
            res += val
        if Tests[scoring_element]:
            score[scoring_element] = max(0,elementval[scoring_element] * correct / len(Tests[scoring_element]))

    total_score =sum(score.values())
    if total_score<100:
        return False, res, score

    return True, res+"\nall tests passed",score


def main_cpp(original_source_file,source_file,program_to_run,original_name,save=False):
    fh = StringIO() if save else sys.stdout

    print('Checking {} for EC327 submission.\n'.format(original_name),file=fh)
    print('Checker Version {0}.{1}'.format(*VERSION),file=fh)

    Grade={}

    the_program = read_file(source_file)
    authors = get_authors(the_program, 'cpp')
    if len(authors)> MAXAUTHORS:
        print("WARNING: You have exceeded the maximum number of authors.",file=fh)


    included = get_includes(the_program)

    #run the specification tests
    print('---- testing your program against the specifications ----',file=fh)


    passed, report, score = blokus_tester(program_to_run)

    print(file=fh)
    print(report,file=fh)
    print(file=fh)

    invalid_includes=False
    for lib in included:
        if lib not in VALID_INCLUDES:
            invalid_includes=True
            print('{} not in valid include list of {}'.format(lib,VALID_INCLUDES),file=fh)
    if invalid_includes:
        Grade['invalid includes'] = -50

    code_metrics = code_analysis_cpp(original_source_file)

    if code_metrics['astyle']=="error":
        print('astyle is reporting a problem.',file=fh)
        code_metrics['astyle']=0

    D = code_metrics['errors']
    cpplint_count= sum(len(D[x]) for x in D)
    

    for fcn in score:
      Grade[fcn] = score[fcn]

    Grade['cpplint'] = max(0, 5-cpplint_count)
    Grade['astyle'] = 1.7*code_metrics['astyle']

    print('---- analysis of your code structure ----\n',file=fh)

    print('authors          : {}'.format(" ".join(authors)
                                               if authors else AUTHWARN),file=fh)
    print('included libs    : {}'.format(" ".join(included)),file=fh)
    print(code_size_report(code_metrics, {'lines': 316, 'words': 1047}),file=fh)

    print("cpplint          : {}".format("{} problems".format(cpplint_count) if cpplint_count else "ok"),file=fh)
    for e in code_metrics['errors']:
        for x in code_metrics['errors'][e][:3]:
            print('line {} ({}): {}'.format(*x),file=fh)
    print("astyle           : {:.1%} code unchanged.\n".format(code_metrics['astyle']),file=fh)


    print('---- grading ----\n',file=fh)

    print('grading scheme\n',file=fh)
    print('alternate assignment points',file=fh)
    print(' * 10 points for "create" and "show" (basic)',file=fh)
    print(' * 5 points for "rotate"',file=fh)
    print(' * 5 points for "fliplr"',file=fh)
    print(' * 5 points for "flipud"',file=fh)
    print(' * 5 points for "board"',file=fh)
    print(' * 5 points for "reset"',file=fh)
    print(' * 10 points for "play"',file=fh)
    print(' * 5 points for "show tiles"',file=fh)
    print(' * 5 points for "resize" (bigger or same)',file=fh)
    print(' * 1.7 points for astyle (% file unchanged by astyle)',file=fh)
    print(' * 5 points for cpplint, -1 deduction for each problem',file=fh)
    print(' * 5 points for moving tiles to upper left',file=fh)

    print('\nfull assignment points',file=fh)
    print(' * 15 points for duplicate tile rejection',file=fh)
    print(' * 10 points for disconnected tile rejection',file=fh)
    print(' * 5 points for "resize" (smaller)',file=fh)
    print(' * 3.3 points for bad tile rejection',file=fh)
    print('\nyour grades:',file=fh)
    for key in elementval:
      try:
        print('  ',key,'[max {}] ='.format(elementval[key]),Grade[key],file=fh)
      except:
        pass
    print('your grade total : {:.2f} / 100'.format(sum(Grade[x] for x in Grade)),file=fh)

    if save:
        return Grade,fh.getvalue()


def cppshell(Parms,q):
      vals = main_cpp(**Parms)
      q.put(vals)

def special_compile(the_program,new_source_file,executable_name):
    if NOBRACKETS:
      bracket_count = the_program.count('[')
      if bracket_count:
         return "You are not allowed to use brackets [].\nUse .at() instead.\nYou have {} brackets.".format(bracket_count),False
    try:
      first_part,second_part=the_program.split(splitter)
    except Exception as e:
      if '\r\n' in the_program:
        return r"Detected windows CRLF newlines in your program. Please use linux '\n' newlines.",False
      else:
        return "Unable to find the marker:\n{}\n".format(splitter),False

    with open(new_source_file,'w') as f:
        f.write(first_part)
        f.write(real_main)
    T = run(["g++","-std=c++14","-O3",new_source_file,"-o",executable_name],stderr=PIPE)
    if T.returncode:
        PScheck=run(['ps','ux'],stdout=PIPE,stderr=PIPE,universal_newlines=True)
        if "blokus_tester" in PScheck.stdout:
          return "Unable to compile {}\nTry running\n\nps ux | grep blokus\n\nin terminal and kill these processes.\nCompiler message:\n".format(new_source_file) + T.stderr.decode(),False
        else:
          return "Unable to compile {}\n".format(new_source_file) + T.stderr.decode(),False

    return "OK",True

if __name__ == '__main__':
    print('\n'*10)
    print('Blokus Checker Version {0}.{1}'.format(*VERSION))
    PD = {'source_file':"blokus.cpp",'program_to_run':'blokus_tester','original_name':'blokus.cpp'}


    with open(PD['source_file']) as f:
        the_program = f.read()
    compmsg, comp_ok = special_compile(the_program,'blokus_main_replaced.cpp',PD['program_to_run'])
    if not comp_ok:
        print(compmsg)
    else:
        main_cpp(PD['source_file'],PD['source_file'],PD['program_to_run'],PD['original_name'])

