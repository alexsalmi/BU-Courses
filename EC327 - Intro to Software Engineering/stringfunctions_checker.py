"""checker for stringfunctions

Note: the author emails are not validated against the actual student list.
"""

# additional tests for string functions

program_commands  = {'is_word':'w','is_palindrome':'p','add':'a','convertbase':'c','multibase':'m'}

error_codes = { program_commands[x]:x for x in program_commands }

# input, output

# is_word
Tests ={x:[] for x in program_commands}
for falsecase in ('AbcdZ',r'`aaa','AA@A','ZZZ[','zzz{z','ninE','tEn','12321','aI','OPERAtor','uP',"abcdefghi1"):
   Tests['is_word'].append( (falsecase,'false'))
for truecase in ('eleven','braces','Six','seven','EIGHT','a','I','Z','z','On','NO'):
  Tests['is_word'].append( (truecase,'true') )


# is_palindrome
for good_palin in ('123321','99999','9876543223456789','9','1','44'): # good palindromes
   Tests['is_palindrome'].append( (good_palin, 'true false') )

for bad_number in ('abc','-123','4.4','012321','123b321','ABBA'): # bad numbers  
   Tests['is_palindrome'].append( (bad_number, 'false true') )

for bad_palin in ('123','3433','3343','9999999999999999991','1222222222'): # good numbers, not palindromes
   Tests['is_palindrome'].append( (bad_palin,'false false') )

# add
Tests['add'] =[
  ('1234 6','1240'),
  ('6 1234','1240'),
  ('123 312','435'),
  ('4 6','10'),
  ('88 99','187'),
  ('6'*234+' '+'7'*236,'78'+'4'*233+'3')
  ]

#convertbase
Tests['convertbase'] = [
  ('11011 2 10','27'),
  ('abc 100 2','1111000110111001011'),
  ('<A;>B 20 18','11;634'),
  (';; 12 10','143')
  ]

#multibase
Tests['multibase'] = [
  ('5',"2 4"),
  ("70","9 13 34 69"),
  ('99',"2 10 32 98"),
  ("11","10"),
  ("39","12 38"),
  ("143","12 142")
  ]

CPPLINT_IGNORE = ['readability/alt_tokens']

COMMENT_STRING = {'py': '#', 'sh': "#", 'cpp': '//'}

AUTHWARN = "WARNING, NO VALID AUTHOR LINES FOUND"

from subprocess import PIPE,Popen,run
import re
import sys
import cpplint
import difflib
from io import StringIO
import time

real_main=r"""// leave this line and everything below as is
int main() {
  bool error;

  cerr << std::boolalpha;
  cout << std::boolalpha;

  // is_word basic tests
  if (not is_word("test")) cerr << "we1\n";
  if (not is_word("Test")) cerr << "we2\n";
  if (not is_word("TEST")) cerr << "we3\n";
  if (not is_word("thisisaword")) cerr << "we4\n";

  if (is_word("123")) cerr << "we5\n";
  if (is_word("")) cerr << "we6\n";
  if (is_word("abc123abc")) cerr << "we7\n";
  if (is_word("tEst")) cerr << "we8\n";
  if (is_word("tEst")) cerr << "we9\n";
  if (is_word("TESTer")) cerr << "we10\n";


  // is_palindrome basic tests
  if (not is_palindrome("12321", &error) or error) cerr << "pe1\n";
  if (not is_palindrome("9009", &error) or error) cerr  << "pe2\n";
  if (not is_palindrome("9", &error) or error)  cerr << "pe3\n";
  if (not is_palindrome("123456777654321", &error) or error) cerr << "pe4\n";

  if (is_palindrome("abcba", &error) or not error) cerr << "pe5\n";
  if (is_palindrome("12321 a", &error) or not error) cerr << "pe6\n";
  if (is_palindrome("0012100", &error) or not error) cerr << "pe7\n";

  if (is_palindrome("123", &error) or error) cerr << "pe8\n";
  if (is_palindrome("123211", &error) or error) cerr << "pe9\n";
  if (not is_palindrome("0", &error) or error)  cerr << "pe10\n";

  // add basic tests
  if (add("123", "456") != "579") cerr << "ae1\n";
  if (add("123", "4") != "127") cerr << "ae2\n";
  if (add("1234", "9") != "1243") cerr << "ae3\n";
  if (add("88", "12") != "100") cerr << "ae4\n";
  if (add("1234567890123456789", "10000000000000999")
      != "1244567890123457788") cerr << "ae5\n";

  string longone(120, '2');
  longone[0] = '3';
  string longother(123, '1');
  longother[0] = '4';
  longother[3] = '2';
  string longresult(123, '3');
  longresult[0] = '4';
  longresult[1] = '1';
  longresult[2] = '1';
  longresult[3] = '5';

  if (add(longone, longother) != longresult ) cerr << "ae6\n";

  // convertbase tests

  if (convertbase("1111", 2, 10) != "15" ) cerr << "ce1\n";
  if (convertbase("255", 10, 16) != "??")  cerr << "ce2\n";
  if (convertbase("755", 8, 2) != "111101101") cerr << "ce3\n";

  if (convertbase("987123", 30, 30) != "987123" ) cerr << "ce4\n";
  if (convertbase("azbc", 100, 10) != "49745051") cerr << "ce5\n";

  if (convertbase("azbc", 100, 2) != "10111101110000110010011011")
    cerr << "ce6\n";

  // multibase tests

  if (multibase(121) != "3 7 8 10 120") cerr << "me1\n";
  if (multibase(45) != "2 8 14 44") cerr << "me2\n";
  if (multibase(63) != "2 4 8 20 62") cerr << "me3\n";
  if (multibase(10) != "3 4 9") cerr << "me4\n";

  string asktype;
  bool res;
  string userinput, num1, num2;
  int mbase, frombase, tobase;



  while (cin >> asktype) {
    if (asktype == "w") {  // is_word
      std::cin >> userinput;
      cout << is_word(userinput) << "\n";
    } else if (asktype == "p") {  // p - is_palindrome
      std::cin >> userinput;
      res = is_palindrome(userinput, &error);
      cout << res << " " << error << "\n";
    } else if (asktype == "a") {  // a - add
      std::cin >> num1 >> num2;
      cout << add(num1, num2) << "\n";
    } else if (asktype == "c") {  // c - convertbase
      std::cin >> userinput >> frombase >> tobase;
      cout << convertbase(userinput, frombase, tobase) << "\n";
    } else if (asktype == "m") {  // m - multibase
      std::cin >> mbase;
      cout << multibase(mbase) << "\n";
    } else {
      return 0;
    }
  }
  return 0;
}
"""


ASTYLE_OPTIONS = [
    '--style=google', '--indent=spaces=2', '--formatted', '--dry-run'
]

VALID_INCLUDES=['iostream','string']


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

def stringfunctions_tester(program_to_run):
    popen_specs={'stdout':PIPE,'stdin':PIPE,'stderr':PIPE,'universal_newlines':True}
    process = Popen([program_to_run],**popen_specs)

    time.sleep(0.02)
    return_code = process.poll()
    if return_code:
        return False,'Your program exited with return code {}.'.format(return_code)


    res = ""
    score = {}

    for function_to_test in sorted(program_commands):
        correct = 0
        incorrect = 0
        res += 'testing {}\n'.format(function_to_test)
        for instring,outstring in Tests[function_to_test]:
            try:
              val = check(outstring, ask_program(process,instring,program_commands[function_to_test]), instring)
            except UnicodeDecodeError:
              val = "problem with case {}\n correct: {}\n   yours: **INVALID ASCII CHARS, CANT DECODE**\n".format(
                       instring,repr(outstring))
      
            if not val:
                correct += 1
            res += val
        score[function_to_test] = max(0,16 * correct / len(Tests[function_to_test]))


    (stdout, stderr) = process.communicate('0\n',timeout=1)
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


def main_cpp(source_file,program_to_run,original_name,faster_than_server=1,save=False):
    fh = StringIO() if save else sys.stdout

    print('Checking {} for EC327 submission.\n'.format(original_name),file=fh)

    Grade={}

    the_program = read_file(source_file)
    authors = get_authors(the_program, 'cpp')
    if len(authors)> 2:
        print("WARNING: You have exceeded the maximum number of authors.",file=fh)


    included = get_includes(the_program)

    #run the specification tests
    print('---- testing your program against the specifications ----',file=fh)
    passed, report, score = stringfunctions_tester(program_to_run)

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

    code_metrics = code_analysis_cpp(source_file)

    if code_metrics['astyle']=="error":
        print('astyle is reporting a problem.',file=fh)
        code_metrics['astyle']=0

    D = code_metrics['errors']
    cpplint_count= sum(len(D[x]) for x in D)
    

    for fcn in score:
      Grade[fcn] = score[fcn]

    Grade['cpplint'] = max(0, 10-2*cpplint_count)
    Grade['astyle'] = 10*code_metrics['astyle']


    print('---- analysis of your code structure ----\n',file=fh)

    print('authors          : {}'.format(" ".join(authors)
                                               if authors else AUTHWARN),file=fh)
    print('included libs    : {}'.format(" ".join(included)),file=fh)
    print(code_size_report(code_metrics, {'lines': 187, 'words': 943}),file=fh)

    print("cpplint          : {}".format("{} problems".format(cpplint_count) if cpplint_count else "ok"),file=fh)
    for e in code_metrics['errors']:
        for x in code_metrics['errors'][e][:3]:
            print('line {} ({}): {}'.format(*x),file=fh)
    print("astyle           : {:.1%} code unchanged.\n".format(code_metrics['astyle']),file=fh)


    print('---- grading ----\n',file=fh)

    print('grading scheme',file=fh)
    print(' * 16 for passing the specifications of each function',file=fh)
    print(' * 10 points for astyle (% file unchanged by astyle)',file=fh)
    print(' * 10 points for cpplint, -2 deduction for each problem',file=fh)

    print('\nyour grades:',file=fh)
    for key in sorted(Grade):
      print('  ',key,Grade[key],file=fh)
    print('your grade total : {:.2f} / 100'.format(sum(Grade[x] for x in Grade)),file=fh)

    if save:
        return Grade,fh.getvalue()


def cppshell(Parms,q):
      vals = main_cpp(**Parms)
      q.put(vals)

def special_compile(the_program,new_source_file,executable_name):
    try:
      splitter = '// leave this line and everything below as is\n'
      first_part,second_part=the_program.split(splitter)
    except Exception as e:
      if '\r\n' in the_program:
        return r"Detected windows CRLF newlines in your program. Please use linux '\n' newlines."
      else:
        return "Unable to find the marker:\n{}\n".format(splitter)

    with open(new_source_file,'w') as f:
        f.write(first_part)
        f.write(real_main)
    T = run(["g++","-std=c++14",new_source_file,"-o",executable_name],stderr=PIPE)
    if T.returncode:
        return str(T)+T.stderr.decode()

if __name__ == '__main__':
    PD = {'source_file':"stringfunctions.cpp",'program_to_run':'stringfunctions','original_name':'stringfunctions.cpp'}
    with open(PD['source_file']) as f:
        the_program = f.read()

    result = special_compile(the_program,'stringfunctions_main_replaced.cpp',PD['program_to_run'])

    main_cpp(PD['source_file'],PD['program_to_run'],PD['original_name'],faster_than_server=1)

