"""checker for happygen_alt

Note: the author emails are not validated against the actual student list.
"""
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

CPPLINT_IGNORE = ['readability/alt_tokens','build/include_subdir']

MAXAUTHORS = 3

AUTHWARN = "WARNING, NO VALID AUTHOR LINES FOUND"


ASTYLE_OPTIONS = [
    '--style=google', '--indent=spaces=2', '--formatted', '--dry-run'
]


REQUIRED_INCLUDES = ['algorithm','iostream','string','vector','timer.h']

VALID_INCLUDES = REQUIRED_INCLUDES + ['array','map','algorithm',"set","unordered_map","unordered_set"]


program_commands  = {'is_happy':'i','find_happy_up_to':'f','happiness_cycle':'c'}

Tests ={x:[] for x in program_commands}

Tests['is_happy'] =[
  ('1 1','1'),
  ('1 4','0'),
  ('1 1324132','1'),
  ('1 4','0'),
  ('2 402 1000','0')
  ]


Tests['find_happy_up_to'] = [
  ('1 20',"1 7 10 13 19"),
  ('1 10',"1 7 10"),
  ('2 100 3','1 3 9 13 17 23 25 27 31 35 37 39 47 51 53 59 61 65 69 71 73 75 77 79 81 85 89 91 93'),
  ('2 10 2',"1 2 3 4 5 6 7 8 9 10"),
  ('2 1000 1000',"1 718 1000"),
  ]


Tests['happiness_cycle'] = [
  ("1 145","145 42 20 4 16 37 58 89"),
  ('2 5 3',"5"),
  ('2 2 3',"2 4"),
  ('2 2 1000',"606866 1117192 50554 309416 268537 360193 166849 748357 686953 1378805 790910 1452200 244305 152561 337825 794194 668072 451408 369865 884386 930452 1069204 46378 145000 21025 1066 4357 127465 232354 179140 51641 413482 402893 959053 922490 1090184 41957 917530 1121789 637163 432338 300868 843424 890425 972725 1470409 388182 183668 479713 737810 1199269 111963 939690 1357821 801491 882682 1243048 61354 129037 18010 424 179776 634217 449045 203626 433085 194714 547432 485833 929114 876037 768745 1144849 741538 838525 977869 1709690 978782 1568008 322689 578405 498109 259885 850306 816136 684352 591760 926881 1633637 806459 860317 840089 713521 779810 1262941 954126 925992 1839689 1178643 445134 215981 1008586 343461 330170 137800 658769 1024325 106202 52040 4304 92432 195088 45769 593386 500645 666025 444181 229897 857050 736949 1442297 283574 409565 486506 492232 295888 875569 1089386 156918 867060 755289 653546 724525 799801 1280002 78405 170109 40781 611561 688042 475108 237289 139690 495421 422266 248840 767104 599105 369826 818437 860093 748249 621505 640666 853156 751945 1457026 209526 320357 229849 773242 656093 438985 1162069 31006 997 994009 988117 989833 1672010 451685 672626 843460 922249 912085 838969 1641205 452907 1026953 908886 1609460 582482 571048 328345 226609 421957 1093090 16750 562756 887380 931169 895322 904709 1319897 906371 958477 1145293 106875 776861 1343497 364659 566777 924085 861001 741322 652765 1010329 108342 128628 410768 757924 1426825 862102 753448 767713 1096658 442181 228125 67609 375370 277525 352354 249220 110401 172901 841385 855506 987061 977890 1746629 952158 931268 938585 1222069 54046 5032 1049 2402 161608 395585 498250 310504 350116 135956 932161 894545 1096261 77338 120173 44329 110177 43429 185890 826325 787901 1431170 214662 484040 235856 787961 1542890 1085865 755451 773426 779005"),
   ('2 7 14',"37 85")]


real_main=r"""
// MAIN   leave this line and all below as is
//        when submitting


const int SLIMIT = 40'000;

void standard_tests();
void interactive_main();

int main(int argc, const char **argv) {
  if (argc > 1)
    interactive_main();
  else
    standard_tests();
  return 0;
}

void standard_tests() {
  double speed = computer_speed();
  std::cerr << "Your computers speed relative to server: " << speed << "\n";

  Timer tall, t0("setup"), t1("all100");
  Timer t3("thousand1"), t4("thousand2");


  std::vector<std::pair<int, int>> res;

  t0.start();
  HappyGenerator h;
  t0.stop();

  // Test case definitions.
  std::vector<int> happy_tests_one{1, 4, 7, 145, 91, 31435135};
  std::vector<std::vector<int>> happy_tests_two{{14500, 10}, {2, 3},
    {255, 2}, {255, 4}, {998, 998}, {4, 10}, {7, 3},
    {41, 100}, {234, 100}, {124, 100}, {22357, 1000}, {1049, 1000}};
  std::vector<std::vector<int>> find_upto_tests_two{{100, 3},
    {1000, 11}, {20, 2}};
  std::vector<int> find_upto_tests_one{10, 100};

  std::cout << "Testing is_happy (two parameters)\n";
  for (auto e : happy_tests_two) {
    std::cout << "is_happy(" << e.at(0) << "," << e.at(1) << ") = ";
    std::cout << h.is_happy(e.at(0), e.at(1)) << "\n";
  }

  std::cout << "Testing is_happy (one parameter)\n";
  for (auto e : happy_tests_one) {
    std::cout << "is_happy(" << e << ") = ";
    std::cout << h.is_happy(e) << "\n";
  }

  std::cout << "Testing happiness_cycle (two parameters)\n";
  for (auto e : happy_tests_two) {
    std::cout << "happiness_cycle(" << e.at(0) << "," << e.at(1) << ") = ";
    for (auto cyc_val : h.happiness_cycle(e.at(0), e.at(1)))
      std::cout << cyc_val << " ";
    std::cout << "\n";
  }

  std::cout << "Testing happiness_cycle (one parameter)\n";
  for (auto e : happy_tests_one) {
    std::cout << "happiness_cycle(" << e << ") = ";
    for (auto cyc_val : h.happiness_cycle(e))
      std::cout << cyc_val << " ";
    std::cout << "\n";
  }

  std::cout << "Testing find_happy_up_to (two parameters)\n";
  for (auto e : find_upto_tests_two) {
    std::cout << "find_happy_up_to(" << e.at(0) << "," << e.at(1) << ") = ";
    for (auto happyval : h.find_happy_up_to(e.at(0), e.at(1)))
      std::cout << happyval << " ";
    std::cout << "\n";
  }


  std::cout << "Testing find_happy_up_to (one parameter)\n";
  for (auto e : find_upto_tests_one) {
    std::cout << "find_happy_up_to(" << e << ") = ";
    for (auto happynum : h.find_happy_up_to(e))
      std::cout << happynum << " ";
    std::cout << "\n";
  }


  std::cout << "Finding the happiest bases\n";

  t1.start();
  for (int i = 2; i < 100; i++) {
    auto v = h.find_happy_up_to(100, i);
    res.push_back(std::make_pair(v.size(), i));
  }
  t1.stop();

  std::sort(res.begin(), res.end());

  std::cout << "The ten happiest bases (for 1 to 100) are \n";
  for (auto it = res.rbegin(); it != res.rbegin() + 10 ; it++)
    std::cout << "base "  << it -> second << " has "
              << it -> first << " happy\n";

  t3.start();
  int count1 = h.find_happy_up_to(SLIMIT).size();
  t3.stop();

  t4.start();
  int count2 = h.find_happy_up_to(SLIMIT).size();
  t4.stop();


  std::cout << "In first " << SLIMIT << " "
            << count1 << " are happy (base 10)\n";

  std::cout << "In first " << SLIMIT << " "
            << count2 << " are happy (base 10)\n";
}


void interactive_main() {
  std::string asktype;
  int number, parameters, base;
  HappyGenerator h;

  while (true) {
    std::cin >> asktype;
    if (asktype == "quit") break;
    std::cin >> parameters >> number;
    if (parameters > 1) std::cin >> base;
    if (asktype == "i" and parameters == 2) {
      std::cout << "is_happy(" << number << "," << base << ") = ";
      std::cout << h.is_happy(number, base) << "\n";
    } else if (asktype == "i" and parameters == 1) {
      std::cout << "is_happy(" << number << ") = ";
      std::cout << h.is_happy(number) << "\n";
    } else if (asktype == "c" and parameters == 2) {
      std::cout << "happiness_cycle(" << number << "," << base << ") = ";
      for (auto cyc_val : h.happiness_cycle(number, base) )
        std::cout << cyc_val << " ";
      std::cout << "\n";
    } else if (asktype == "c" and parameters == 1) {
      std::cout << "happiness_cycle(" << number << ") = ";
      for (auto cyc_val : h.happiness_cycle(number))
        std::cout << cyc_val << " ";
      std::cout << "\n";
    } else if (asktype == "f" and parameters == 2) {
      std::cout << "find_happy_up_to(" << number << "," << base << ") = ";
      for (auto happyval : h.find_happy_up_to(number, base))
        std::cout << happyval << " ";
      std::cout << "\n";
    } else if (asktype == "f" and parameters == 1) {
      std::cout << "find_happy_up_to(" << number << ") = ";
      for (auto happynum : h.find_happy_up_to(number))
        std::cout << happynum << " ";
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

def read_file_for_cpplint(filename):
    "read the contents of filename into list of strings"
    filehand = open(filename)
    contents = filehand.read()
    filehand.close()
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



    return {'lines': num_lines, 'words': num_words, 'errors':Errors,'astyle':unchanged}



SIZE_REPORT_TEMPLATE = """lines of code    : {}, {:4.0%} of reference
tokens in code   : {}, {:4.0%} of reference"""


def code_size_report(submitted_code, reference_code):
    "generate message about code size"
    return SIZE_REPORT_TEMPLATE.format(
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


@timeout(30)
def happygen_tester(program_to_run):

    res = ""
    score = {}

    # standard_tests
    try:
        T = run([program_to_run],timeout=30,**popen_specs)
    except:
        return False, "Your program exceeded 30 second maximum.",score

    h = hashlib.sha256()
    h.update(T.stdout.encode())
    summary = h.hexdigest()  



    res += "running standard_tests(): "
    if summary == '2363ddd9b86d6bbecfb4b015447fa413ad37eae50ae4088a534d7aaf6b8e7577':
       score['standard_tests'] = 45
       res += " all OK\n"
    else:
        score['standard_tests'] = 0
        res += " no match. Compare to answers_alt.txt\n"       


    # interactive_main testing

    process = Popen([program_to_run,"interactive"],**popen_specs)

    time.sleep(0.02)
    return_code = process.poll()
    if return_code:
        return False,'Your program exited with return code {}.'.format(return_code)



    for function_to_test in sorted(program_commands):
        correct = 0
        incorrect = 0
        res += 'testing {}\n'.format(function_to_test)
        for instring,answer in Tests[function_to_test]:
            outstring = '{}({}) = {}'.format(function_to_test,",".join(instring.split()[1:]),answer)
            try:
              val = check(outstring, ask_program(process,instring,program_commands[function_to_test]), instring)
            except UnicodeDecodeError:
              val = "problem with case {}\n correct: {}\n   yours: **INVALID ASCII CHARS, CANT DECODE**\n".format(
                       instring,repr(outstring))
      
            if not val:
                correct += 1
            res += val
        score[function_to_test] = max(0,15 * correct / len(Tests[function_to_test]))


    try:
        (stdout, stderr) = process.communicate('quit\n',timeout=1)
    except BrokenPipeError:
        res += "Your program crashed.\n"
        return False, res, score
    
    if stdout:
        score['exit problem']=-20
        res = res+"\nResponding to exit signal"+repr(stdout)
    elif stderr:
        error_lines = stderr.splitlines()
        for line in error_lines:
            code = line[0]
            if code not in error_codes:
                score['extra error output'] = -20
            else:
                score[error_codes[code]] -= 1

        return False, res+"\nBase errors detected (-1 for each from function score):"+stderr,score

    total_score =sum(score.values())
    if total_score<100:
        return False, res, score

    return True, res+"\nall tests passed",score


def main_cpp(original_source_file,source_file,program_to_run,original_name,save=False):
    fh = StringIO() if save else sys.stdout

    print('Checking {} for EC327 submission.\n'.format(original_name),file=fh)

    Grade={}

    the_program = read_file(source_file)
    authors = get_authors(the_program, 'cpp')
    if len(authors)> MAXAUTHORS:
        print("WARNING: You have exceeded the maximum number of authors.",file=fh)


    included = get_includes(the_program)

    #run the specification tests
    print('---- testing your program against the specifications ----',file=fh)


    passed, report, score = happygen_tester(program_to_run)

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
    print(code_size_report(code_metrics, {'lines': 203, 'words': 946}),file=fh)

    print("cpplint          : {}".format("{} problems".format(cpplint_count) if cpplint_count else "ok"),file=fh)
    for e in code_metrics['errors']:
        for x in code_metrics['errors'][e][:3]:
            print('line {} ({}): {}'.format(*x),file=fh)
    print("astyle           : {:.1%} code unchanged.\n".format(code_metrics['astyle']),file=fh)


    print('---- grading ----\n',file=fh)

    print('grading scheme',file=fh)
    print(' * 45 points for passing the standard_tests in main()',file=fh)
    print(' * 15 points for passing the specifications of is_happy',file=fh)
    print(' * 15 points for passing the specifications of happiness_cycle',file=fh)
    print(' * 15 points for passing the specifications of find_happy_up_to',file=fh)
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
    try:
      splitter = '// MAIN'
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
        return "Unable to compile {}\n".format(new_source_file) + T.stderr.decode(),False

    return "OK",True

if __name__ == '__main__':
    PD = {'source_file':"happygen_alt.cpp",'program_to_run':'happygen_alt_tester','original_name':'happygen_alt.cpp'}


    with open(PD['source_file']) as f:
        the_program = f.read()
    compmsg, comp_ok = special_compile(the_program,'happygenerator_main_replaced.cpp',PD['program_to_run'])
    if not comp_ok:
        print(compmsg)
    else:
        main_cpp(PD['source_file'],PD['source_file'],PD['program_to_run'],PD['original_name'])

