"""checker for arrangements

Note: the author emails are not validated against the actual student list.
"""

VERSION = (2,0)

DEBUG = True

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

REQUIRED_INCLUDES = ['algorithm','iomanip','iostream','string','vector','timer.h']

VALID_INCLUDES = REQUIRED_INCLUDES + ['array','cmath','cstdint','tuple','map','algorithm',"set","unordered_map","unordered_set"]

program_commands  = {
'panel_count':'pc',
'panel_shuffles':'ps',
'dinner_count':'dc',
"dinner_shuffles":"ds"}
fcnval  = {
'panel_count':10,
'panel_shuffles':15,
'dinner_count':10,
"dinner_shuffles":15}

Tests ={x:[] for x in program_commands}

Tests['panel_count']=[
  ('abc12 1','1'),
  ('abc12 2','2'),
  ('abc12 3','3'),
  ('!@##% 4','5'),
  ("a"*100+"b 100",'5.73147844013819e+20'),
  ("a"*26+" 26","196418")
  ]
Tests['dinner_count']=[
  ('abc12 1','1'),
  ('abc12 2','2'),
  ('abc12 3','6'),
  ('!@#&% 4','9'),
  ("a"*100+"b 100",'7.92070839848375e+20'),
  ("a"*26+" 26","271445")
  ]
Tests['panel_shuffles']=[
  ('abc12 2','9b1e2b25a9'),
  ('abc12 3','4b9641e0ab'),
  ('abc12 5','1c0fb84682'),
  ('12345abcdeZ 10','1cc193a1c5'),
  ]

Tests['dinner_shuffles']=[
  ('abc12 2','9b1e2b25a9'),
  ('abc12 3','f555053768'),
  ('abc12 5','c68c8e0efc'),
  ('12345abcdeZ 10','6596db9480'),
  ]

speed_answers={}

speed_answers['panel_shuffles']={
        21:'50e0c68665',
        22:'1e9d3a4157',
        23:'76d9b42b2f',
        24:'0e4f07da64',
        25:'53a97513d9',
        26:"80dd010b6d",
        27:"fc1a84626c",
        28:"77dde6331d",
        29:"c3d4e8e940",
        30:"b874a8550d"
        }


speed_answers['dinner_shuffles']={
        21:'a735edf895',
        22:'6622ec19f0',
        23:'f941784fa8',
        24:'b358fcc8f9',
        25:'53a97513d9',
        25:'a49daa40a6',
        26:"5e16ce1e05",
        27:"4820117664",
        28:"6840381969",
        29:"5122bca5e1",
        30:"2f27ca3bc2"        }

my_basic_answers = ['abcd\nabdc\nacbd\nbacd\nbadc', 'abcd\nabdc\nacbd\nbacd\nbadc\nbcda\ndabc\ndbca\ndcba', 'abcdef\nabcdfe\nabcedf\nabdcef\nabdcfe\nacbdef\nacbdfe\nacbedf\nbacdef\nbacdfe\nbacedf\nbadcef\nbadcfe\n\n123456\n123465\n123546\n124356\n124365\n132456\n132465\n132546\n213456\n213465\n213546\n214356\n214365\n\n!#@$%^\n!#@$^%\n!#@%$^\n!@#$%^\n!@#$^%\n!@#%$^\n!@$#%^\n!@$#^%\n@!#$%^\n@!#$^%\n@!#%$^\n@!$#%^\n@!$#^%', 'abcdef\nabcdfe\nabcedf\nabdcef\nabdcfe\nacbdef\nacbdfe\nacbedf\nbacdef\nbacdfe\nbacedf\nbadcef\nbadcfe\nbcdefa\nfabcde\nfbcdea\nfbceda\nfbdcea\nfcbdea\nfcbeda\n\n123456\n123465\n123546\n124356\n124365\n132456\n132465\n132546\n213456\n213465\n213546\n214356\n214365\n234561\n612345\n623451\n623541\n624351\n632451\n632541\n\n!#@$%^\n!#@$^%\n!#@%$^\n!@#$%^\n!@#$^%\n!@#%$^\n!@$#%^\n!@$#^%\n@!#$%^\n@!#$^%\n@!#%$^\n@!$#%^\n@!$#^%\n@#$%^!\n^!@#$%\n^#@$%!\n^#@%$!\n^@#$%!\n^@#%$!\n^@$#%!']

my_panel_count_answers = [['1', '1'], ['2', '2'], ['3', '3'], ['4', '5'], ['5', '8'], ['6', '13'], ['7', '21'], ['8', '34'], ['9', '55'], ['10', '89'], ['11', '144'], ['12', '233'], ['13', '377'], ['14', '610'], ['15', '987'], ['16', '1597'], ['17', '2584'], ['18', '4181'], ['19', '6765'], ['20', '10946'], ['21', '17711'], ['22', '28657'], ['23', '46368'], ['24', '75025'], ['25', '121393'], ['26', '196418'], ['27', '317811'], ['28', '514229'], ['29', '832040'], ['30', '1346269'], ['31', '2178309'], ['32', '3524578'], ['33', '5702887'], ['34', '9227465'], ['35', '14930352'], ['36', '24157817'], ['37', '39088169'], ['38', '63245986'], ['39', '102334155'], ['40', '165580141'], ['41', '267914296'], ['42', '433494437'], ['43', '701408733'], ['44', '1134903170'], ['45', '1836311903'], ['46', '2971215073'], ['47', '4807526976'], ['48', '7778742049'], ['49', '12586269025'], ['50', '20365011074'], ['51', '32951280099'], ['52', '53316291173'], ['53', '86267571272'], ['54', '139583862445'], ['55', '225851433717'], ['56', '365435296162'], ['57', '591286729879'], ['58', '956722026041'], ['59', '1548008755920'], ['60', '2504730781961'], ['61', '4052739537881'], ['62', '6557470319842'], ['63', '10610209857723'], ['64', '17167680177565'], ['65', '27777890035288'], ['66', '44945570212853'], ['67', '72723460248141'], ['68', '117669030460994'], ['69', '190392490709135'], ['70', '308061521170129'], ['71', '498454011879264'], ['72', '806515533049393'], ['73', '1.30496954492866e+15'], ['74', '2.11148507797806e+15'], ['75', '3.41645462290672e+15'], ['76', '5.52793970088477e+15'], ['77', '8.94439432379149e+15'], ['78', '1.44723340246763e+16'], ['79', '2.34167283484677e+16'], ['80', '3.7889062373144e+16'], ['81', '6.13057907216118e+16'], ['82', '9.91948530947558e+16'], ['83', '1.60500643816368e+17'], ['84', '2.59695496911123e+17'], ['85', '4.20196140727491e+17'], ['86', '6.79891637638614e+17'], ['87', '1.10008777836611e+18'], ['88', '1.77997941600472e+18'], ['89', '2.88006719437082e+18'], ['90', '4.66004661037554e+18'], ['91', '7.54011380474637e+18'], ['92', '1.22001604151219e+19'], ['93', '1.97402742198683e+19'], ['94', '3.19404346349902e+19'], ['95', '5.16807088548585e+19'], ['96', '8.36211434898487e+19'], ['97', '1.35301852344707e+20'], ['98', '2.18922995834556e+20'], ['99', '3.54224848179263e+20']]
my_dinner_count_answers = [['1', '1'], ['2', '2'], ['3', '6'], ['4', '9'], ['5', '13'], ['6', '20'], ['7', '31'], ['8', '49'], ['9', '78'], ['10', '125'], ['11', '201'], ['12', '324'], ['13', '523'], ['14', '845'], ['15', '1366'], ['16', '2209'], ['17', '3573'], ['18', '5780'], ['19', '9351'], ['20', '15129'], ['21', '24478'], ['22', '39605'], ['23', '64081'], ['24', '103684'], ['25', '167763'], ['26', '271445'], ['27', '439206'], ['28', '710649'], ['29', '1149853'], ['30', '1860500'], ['31', '3010351'], ['32', '4870849'], ['33', '7881198'], ['34', '12752045'], ['35', '20633241'], ['36', '33385284'], ['37', '54018523'], ['38', '87403805'], ['39', '141422326'], ['40', '228826129'], ['41', '370248453'], ['42', '599074580'], ['43', '969323031'], ['44', '1568397609'], ['45', '2537720638'], ['46', '4106118245'], ['47', '6643838881'], ['48', '10749957124'], ['49', '17393796003'], ['50', '28143753125'], ['51', '45537549126'], ['52', '73681302249'], ['53', '119218851373'], ['54', '192900153620'], ['55', '312119004991'], ['56', '505019158609'], ['57', '817138163598'], ['58', '1322157322205'], ['59', '2139295485801'], ['60', '3461452808004'], ['61', '5600748293803'], ['62', '9062201101805'], ['63', '14662949395606'], ['64', '23725150497409'], ['65', '38388099893013'], ['66', '62113250390420'], ['67', '100501350283431'], ['68', '162614600673849'], ['69', '263115950957278'], ['70', '425730551631125'], ['71', '688846502588401'], ['72', '1.11457705421953e+15'], ['73', '1.80342355680793e+15'], ['74', '2.91800061102745e+15'], ['75', '4.72142416783538e+15'], ['76', '7.63942477886283e+15'], ['77', '1.23608489466982e+16'], ['78', '2.0000273725561e+16'], ['79', '3.23611226722592e+16'], ['80', '5.23613963978203e+16'], ['81', '8.47225190700795e+16'], ['82', '1.370839154679e+17'], ['83', '2.21806434537979e+17'], ['84', '3.58890350005879e+17'], ['85', '5.80696784543858e+17'], ['86', '9.39587134549738e+17'], ['87', '1.5202839190936e+18'], ['88', '2.45987105364333e+18'], ['89', '3.98015497273693e+18'], ['90', '6.44002602638026e+18'], ['91', '1.04201809991172e+19'], ['92', '1.68602070254975e+19'], ['93', '2.72803880246146e+19'], ['94', '4.41405950501121e+19'], ['95', '7.14209830747268e+19'], ['96', '1.15561578124839e+20'], ['97', '1.86982561199566e+20'], ['98', '3.02544139324405e+20'], ['99', '4.8952670052397e+20']]


real_main=r"""
// TESTING: leave this line and below as is.

void show_result(vector<string> v) {
  sort(v.begin(), v.end());
  for (auto c : v)
    cout << c << "\n";
  cout << "\n";
}

void show_partial_result(string testname, vector<string> res, int n) {
  if (res.empty()) return;

  sort(res.begin(), res.end());

  std::vector<uint64_t> locs{0, res.size() / 3,
                             2 * res.size() / 3, res.size() - 1};
  std::cout << "\n" << testname << " " << n << "\n";
  for (auto i : locs) {
    std::cout << " res.at(" << i
              << ") = " << res.at(i) << "\n";
  }
}


const int COUNTLIM = 100;
const int COUNTLIM_SMALL = 30;

void standard_tests();
void alternate_tests();
void interactive_main();

int main(int argc, char const ** argv) {
  if (argc > 1 and string(*(argv+1)) == "alt")
    alternate_tests();
  else if (argc > 1 and (string(*(argv+1)) == string("int")))
    interactive_main();
  else
    standard_tests();
}

// tests to be run for full credit, including performance.
void standard_tests() {
  int n;

  cout.precision(15);

  // Basic test
  Arrangements standard;

  cout << "\nPanel Shuffles for 4 panelists.\n";
  show_result(standard.panel_shuffles(4));

  cout << "\nDinner Shuffles for 4 guests.\n";
  show_result(standard.dinner_shuffles(4));

  // Test other names
  Arrangements numbers("123456789");
  Arrangements symbols("!@#$%^&*()_+");

  std::array<Arrangements*, 3> v{&standard, &numbers, &symbols};

  cout << "\nPanel Shuffles for 6 panelists, 3 sets of names.\n";
  for (auto arr : v)
    show_result(arr->panel_shuffles(6));

  cout << "\nDinner Shuffles for 6 guests, 3 sets of names.\n";
  for (auto arr : v)
    show_result(arr->dinner_shuffles(6));

  // Count tests
  Arrangements large(string(COUNTLIM, 'a'));

  Timer t_pc("panel count", true);
  n = 1;
  cout << "\nPanel Shuffle Count Table (0.1 seconds)\n";
  cout << "     N  panel(N)\n";

  while (n < COUNTLIM and t_pc.time() < 0.1) {
    t_pc.start();
    double pc = large.panel_count(n);
    t_pc.stop();
    cout << std::setw(6) << n << " "
         << std::setw(6) << pc << "\n";
    n++;
  }


  Timer t_dc("dinner count", true);
  n = 1;
  cout << "\nDinner Shuffle Count Table (0.1 seconds)\n";
  cout << "     N  dinner(N)\n";

  while (n < COUNTLIM and t_dc.time() < 0.1) {
    t_dc.start();
    double dc = large.dinner_count(n);
    t_dc.stop();
    cout << std::setw(6) << n << " "
         << std::setw(6) << dc << "\n";
    n++;
  }

  Timer t_panel("panel", true);
  n = 4;
  cout << "\nHow many panel shuffles can be created in 0.5 seconds?\n";

  while (t_panel.time() < 0.5 and n <= 30)  {
    double last = t_panel.time();
    t_panel.start();
    vector<string> res = standard.panel_shuffles(n);
    t_panel.stop();
    show_partial_result("panel", res, n);
    cout << "time " << t_panel.time() - last << "\n";
    n++;
  }

  int largest_panel = n - 1;

  Timer t_dinner("dinner timing", true);
  n = 4;
  cout << "\nHow many dinner shuffles can be created in 0.5 seconds?\n";

  while (t_dinner.time() < 0.5 and n <= 30)  {
    double last = t_dinner.time();
    t_dinner.start();
    vector<string> res = standard.dinner_shuffles(n);
    t_dinner.stop();
    show_partial_result("dinner", res, n);
    cout << "time " << t_dinner.time() - last << "\n";
    n++;
  }
  cout << "\nLargest panel shuffles performed: "
       << largest_panel << "\n";
  cout << "\nLargest dinner shuffles performed: " << n - 1 << "\n";

  // Error checking
  Arrangements small("abcd");
  cout << "\nError Handling Tests\n";

  try {
    small.panel_count(5);
  } catch (int n) {
    cout << n;
  }
  try {
    small.panel_shuffles(6);
  } catch (int n) {
    cout << n;
  }
  try {
    small.dinner_count(7);
  } catch (int n) {
    cout << n;
  }
  try {
    small.dinner_shuffles(89);
  } catch (int n) {
    cout << n;
  }
  try {
    large.dinner_shuffles(122);
  } catch (int n) {
    cout << n;
  }
  try {
    numbers.dinner_shuffles(9);
  } catch (int n) {
    cout << n;
  }
  try {
    numbers.dinner_shuffles(10);
  } catch (int n) {
    cout << n;
  }
  cout << "\n";
}

void alternate_tests() {
  cout.precision(15);
  // Basic test
  Arrangements standard;

  cout << "\nPanel Shuffles for 4 panelists.\n";
  show_result(standard.panel_shuffles(4));

  cout << "\nPanel Shuffles for 10 panelists.\n";
  show_result(standard.panel_shuffles(10));

  int n = 1;

  // Count tests
  Timer t_pc("panel count", true);
  cout << "\nPanel Shuffle Count Table (0.1 seconds)\n";
  cout << "     N  panel(N)\n";
  while (n < 52 and t_pc.time() < 0.1) {
    t_pc.start();
    double pc = standard.panel_count(n);
    t_pc.stop();
    cout << std::setw(6) << n << " "
         << std::setw(6) << pc << "\n";
    n++;
  }

  cout << "\nHow many panel shuffles can be created in 0.5 seconds?\n";
  n = 4;

  Timer t_panel("panel", true);

  while (t_panel.time() < 0.5)  {
    t_panel.start();
    vector<string> res = standard.panel_shuffles(n);
    t_panel.stop();
    show_partial_result("panel", res, n);
    n++;
  }
  cout << "\nLargest panel shuffles performed: "
       << n - 1 << "\n";
}


void interactive_main() {
  std::string asktype, symbols;
  int number;
  cout << "Type quit to exit.\n";
  cout << "Commands:\npc names n\nps names n\ndc names n\nds names n\n";
  cout.precision(15);

  while (true) {
    std::cin >> asktype;
    if (asktype == "quit") break;
    std::cin >> symbols;
    Arrangements h(symbols);
    std::cin >> number;
    if (asktype == "pc") {
      std::cout << "panel_count(" << number <<  ") = ";
      std::cout << h.panel_count(number) << "\n";
    } else if (asktype == "ps") {
      std::cout << "panel_shuffles(" << number <<  ") = ";
      for (auto e : h.panel_shuffles(number) )
        std::cout << e << " ";
      std::cout << "\n";
    } else if (asktype == "dc") {
      std::cout << "dinner_count(" << number << ") = ";
      std::cout << h.dinner_count(number) << "\n";
    } else if (asktype == "ds") {
      std::cout << "dinner_shuffles(" << number <<  ") = ";
      for (auto e : h.dinner_shuffles(number))
        std::cout << e << " ";
      std::cout << "\n";
    }
  }
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



def ask_program(process,case,command):
    process.stdin.write(command+'\n')
    process.stdin.write(case+'\n')
    process.stdin.flush()
    return process.stdout.readline().strip()


def check(answer,youranswer,case):
    if youranswer != answer:
        return "problem with case {}\n correct: {}\n   yours: {}\n".format(case,repr(answer),repr(youranswer))   
    return ""

def hexcheck(answer,youranswer,case):
    phrase, myhex = answer.rsplit("= ",1)
    phrase, yourval = youranswer.rsplit("= ",1)
    yourval = yourval.split()
    yourval.sort()
    yourval = "\n".join(yourval)

    h = hashlib.sha256()
    h.update(yourval.encode())
    summary = h.hexdigest()[:10]

    if summary != myhex:
        return "problem with case {}. My digest: {}, your digest: {}\n".format(phrase,myhex,summary)
    return ""

def numbercheck(answer,youranswer,case):
    phrase,val = answer.rsplit(" ",1)
    phrase,yourval = youranswer.rsplit(" ",1)

    if not numpy.isclose(float(yourval),float(val)):
        return "problem with case {}\n correct: {}\n   yours: {}\n".format(case,repr(answer),repr(youranswer))   
    return ""

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
    splitter = '// TESTING: leave this line and below as is.'
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

output_headings = ["Panel Shuffles for 4 panelists.",
"Dinner Shuffles for 4 guests.",
"Panel Shuffles for 6 panelists, 3 sets of names.",
"Dinner Shuffles for 6 guests, 3 sets of names.",
"Panel Shuffle Count Table (0.1 seconds)",
"Dinner Shuffle Count Table (0.1 seconds)",
"How many panel shuffles can be created in 0.5 seconds?",
"How many dinner shuffles can be created in 0.5 seconds?",
"Largest panel shuffles performed:",
"Largest dinner shuffles performed:",
"Error Handling Tests"]

ve_str = "your program seems to have crashed in {} using the interactive command: {} {}\n"

@timeout(15)
def arrangements_tester(program_to_run):

    res = ""
    score = {}

    # standard_tests
    if DEBUG:
      print('running standard tests')
    try:
        T = run([program_to_run],timeout=12,**popen_specs)
    except:
        return False, "Your program exceeded 12 second maximum.\nRun arrangements_tester to investigate.",score
   
    output = T.stdout
    stuff=[]
    try:
        for heading in output_headings:
            first,second = output.split(heading)
            stuff.append(first.strip())
            output = second
    except:
        for s in stuff:
            print(len(s))
            print(output)
        return False,"Unable to find the following heading in your output: {} \n".format(heading),score

    stuff.append(second.strip())
    stuff = stuff[1:]
    basics = stuff[:4]
    panel_table =[x.strip().split() for x in stuff[4].splitlines()[1:]]
    dinner_table = [x.strip().split() for x in stuff[5].splitlines()[1:]]
    panel_shuffles = stuff[6]
    dinner_shuffles = stuff[7]
    try:
      largest={'panel_shuffles': min(30,int(stuff[8])),'dinner_shuffles':min(30,int(stuff[9]))}
    except:
      return False,"Unable to parse your output, tried to interpret\n{}\nand\n{} as numbers.".format(stuff[8],stuff[9])
    errors = stuff[10]

    res += "running standard_tests():\n"
 

    if basics != my_basic_answers:
        res += "Basic tests failing. See arrangements_answers.txt.\n"
    if panel_table[:72] != my_panel_count_answers[:72]:
        score['panel_count'] = 0
        res += "Your panel count table must match exactly up to panel(72).\n"
    if dinner_table[:72] != my_dinner_count_answers[:72]:
        score['dinner_count'] = 0
        res += "Your dinner count table must match exactly up to dinner(72).\n"
    if errors == '5678912210':
        score['error_handling'] = 10



    checkfcn = {'panel_count':numbercheck,'panel_shuffles':hexcheck,
                'dinner_count':numbercheck,"dinner_shuffles":hexcheck}


    # interactive_main testing

    process = Popen([program_to_run,"int"],**popen_specs)
    for i in range(6):
        process.stdout.readline()

    time.sleep(0.02)
    return_code = process.poll()
    if return_code:
        return False,'Your program exited with return code {}.'.format(return_code)


    for function_to_test in sorted(program_commands):
        correct = 0
        incorrect = 0
        res += 'testing {}\n'.format(function_to_test)
        for instring,answer in Tests[function_to_test]:
            if DEBUG:
              print('testing',function_to_test,instring)
            outstring = '{}({}) = {}'.format(function_to_test,",".join(instring.split()[1:]),answer)
            try:

              val = checkfcn[function_to_test](outstring, ask_program(process,instring,program_commands[function_to_test]), instring)
            except UnicodeDecodeError:
              val = "problem with case {}\n correct: {}\n   yours: **INVALID ASCII CHARS, CANT DECODE**\n".format(
                       instring,repr(outstring))
            except BrokenPipeError:
              val = "broken pipe crash on test: {} \n".format(instring)
            except ValueError:
              val = ve_str.format(function_to_test,program_commands[function_to_test],instring)
      
            if not val:
                correct += 1
            res += val
        if Tests[function_to_test]:
            score[function_to_test] = max(0,fcnval[function_to_test] * correct / len(Tests[function_to_test]))

    print('checking speed results.')
    res += "checking speed results\n"
    if score['dinner_shuffles'] != fcnval['dinner_shuffles'] or  score['panel_shuffles'] != fcnval['panel_shuffles']:
        res += "Your program is incorrect, no speed score can be assigned.\n"
        score['program_speed'] = 0
    elif largest['dinner_shuffles']<20 or largest['panel_shuffles']<20:
        res += "You must get to a threshold of 20 shuffles for both panel and dinner to get a speed score.\n"
        score['program_speed'] = 0
    else:
        speed_ok = True
        for function_to_test in ['panel_shuffles','dinner_shuffles']:
            answer = speed_answers[function_to_test][largest[function_to_test]-2]
            res += "Verifying your correct result for {} at {}\n".format(function_to_test,largest[function_to_test]-2)
            outstring = '{}({}) = {}'.format(function_to_test,",".join(instring.split()[1:]),answer)
            instring ="{} {}".format(string.ascii_letters,largest[function_to_test]-2)
            print('testing',function_to_test,instring)
            val = checkfcn[function_to_test](outstring, ask_program(process,instring,program_commands[function_to_test]), instring)
            res += val
            if val:
                speed_ok = False

        if speed_ok:
            total_done = sum(int(largest[x]) for x in largest)
            res += "You did {} shuffles in 1 second.\n".format(total_done)
            if total_done>=54:
                res += "Nice work, full credit for score.\n"
                score['program_speed'] = 30
            else:

                score['program_speed'] = (total_done-40)**2 *30 / 14**2


        else:
            res += "There is a problem, so no speed score.\n"
            score['program_speed'] = 0



    if DEBUG:
       print('done with tests, closing.')

    try:
        (stdout, stderr) = process.communicate('quit\n',timeout=1)
    except BrokenPipeError:
        res += "Your program crashed.\n"
        return False, res, score
    
    if stdout:
        score['exit problem']=-20
        res = res+"\nResponding to exit signal"+repr(stdout)
    elif stderr:
        return False, res+"\nError messages when running in interactive mode:\n"+stderr,score

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


    passed, report, score = arrangements_tester(program_to_run)

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

    Grade['cpplint [max 5]'] = max(0, 10-2*cpplint_count)/2
    Grade['astyle [max 5]'] = 5*code_metrics['astyle']

    print('---- analysis of your code structure ----\n',file=fh)

    print('authors          : {}'.format(" ".join(authors)
                                               if authors else AUTHWARN),file=fh)
    print('included libs    : {}'.format(" ".join(included)),file=fh)
    print(code_size_report(code_metrics, {'lines': 158, 'words': 556}),file=fh)

    print("cpplint          : {}".format("{} problems".format(cpplint_count) if cpplint_count else "ok"),file=fh)
    for e in code_metrics['errors']:
        for x in code_metrics['errors'][e][:3]:
            print('line {} ({}): {}'.format(*x),file=fh)
    print("astyle           : {:.1%} code unchanged.\n".format(code_metrics['astyle']),file=fh)


    print('---- grading ----\n',file=fh)

    print('grading scheme',file=fh)
    print(' * 10 points for passing the specifications of panel_count',file=fh)
    print(' * 15 points for passing the specifications of panel_shuffles',file=fh)
    print(' * 10 points for passing the specifications of dinner_count',file=fh)
    print(' * 15 points for passing the specifications of dinner_shuffles',file=fh)
    print(' * 10 points for error handling',file=fh)
    print(' * 30 points for program speed',file=fh)
    print(' * 5 points for astyle (% file unchanged by astyle)',file=fh)
    print(' * 5 points for cpplint, -1 deduction for each problem',file=fh)

    print('\nyour grades:',file=fh)
    for key in sorted(Grade):
      print('  ',key,'=',Grade[key],file=fh)
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
      splitter = '// TESTING: leave this line and below as is.'
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
        if "arrangements_tester" in PScheck.stdout:
          return "Unable to compile {}\nTry running\n\nps ux | grep arrangements\n\nin terminal and kill these processes.\nCompiler message:\n".format(new_source_file) + T.stderr.decode(),False
        else:
          return "Unable to compile {}\n".format(new_source_file) + T.stderr.decode(),False

    return "OK",True

if __name__ == '__main__':
    print('Arrangements Checker Version {0}.{1}'.format(*VERSION))
    PD = {'source_file':"arrangements.cpp",'program_to_run':'arrangements_tester','original_name':'arrangements.cpp'}


    with open(PD['source_file']) as f:
        the_program = f.read()
    compmsg, comp_ok = special_compile(the_program,'arrangements_main_replaced.cpp',PD['program_to_run'])
    if not comp_ok:
        print(compmsg)
    else:
        main_cpp(PD['source_file'],PD['source_file'],PD['program_to_run'],PD['original_name'])

