"""checker for happy

Note: the author emails are not validated against the actual student list.
"""

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

happy_list = [1,10,7,2221,10211000,2147483564,2147483645,234756,100,1000,10000,100000,1000000,10000000]
unhappy_list = [4,2000000,2147483647,134134,2341234,45856875,999991,2,123456789,919999,3,5,6,11]
Tests=[]
for x in happy_list:
    Tests.append ( (str(x), "{} is happy".format(x) ) )
for x in unhappy_list:
    Tests.append ( (str(x), "{} is not happy".format(x)))


ASTYLE_OPTIONS = [
    '--style=google', '--indent=spaces=2', '--formatted', '--dry-run'
]

VALID_INCLUDES=['iostream','cstdint']


def ask_happy(process,case):
    process.stdin.write(case+'\n')
    process.stdin.flush()
    return process.stdout.readline().strip()

def check(answer,youranswer,case):
    res = ""
    if youranswer != answer:
        res += "Case {}: correct: '{}', yours: '{}'\n".format(case,answer,youranswer)
    return res

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



SIZE_REPORT_TEMPLATE = """lines of code    : {}, {:4.0%} of reference
tokens in code   : {}, {:4.0%} of reference"""


def code_size_report(submitted_code, reference_code):
    "generate message about code size"
    return SIZE_REPORT_TEMPLATE.format(
        submitted_code['lines'],
        submitted_code['lines'] / reference_code['lines'],
        submitted_code['words'],
        submitted_code['words'] / reference_code['words'])



def happy_tester(program_to_run):
    popen_specs={'stdout':PIPE,'stdin':PIPE,'universal_newlines':True}
    process = Popen([program_to_run],**popen_specs)

    time.sleep(0.02)
    return_code = process.poll()
    if return_code:
        return False,'Your program exited with return code {}.'.format(return_code)


    res = ""
    correct = 0
    incorrect = 0

    for case,answer in Tests:
        res += 'testing {}\n'.format(case)
        val = check(answer, ask_happy(process,case), case)
        if val:
            incorrect += 1
        else:
            correct += 1
        res += val

    score = max(0,100 * (correct - incorrect) / len(Tests))


    (stdout, stderr) = process.communicate('0\n',timeout=1)
    if stdout != "":
        return False, res+"\nResponding to exit signal",score-20
    elif stderr != None:
        return False, res+"\nExtra output to stderr.",score-20

    if score < 100:
        return False, res, score

    return True, res+"\nall tests passed",score


def main_cpp(source_file,program_to_run,original_name,faster_than_server=1,save=False):
    fh = StringIO() if save else sys.stdout

    print('Checking {} for EC327 submission.\n'.format(original_name),file=fh)

    Grade={'specs':0}

    the_program = read_file(source_file)
    authors = get_authors(the_program, 'cpp')
    if len(authors)> 2:
        print("WARNING: You have exceeded the maximum number of authors.",file=fh)


    included = get_includes(the_program)

    #run the specification tests
    print('---- testing your program against the specifications ----',file=fh)
    passed, report, score = happy_tester(program_to_run)

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
    

    Grade['specs'] = 0.8 * score

    Grade['cpplint'] = max(0, 10-2*cpplint_count)
    Grade['astyle'] = 10*code_metrics['astyle']


    print('---- analysis of your code structure ----\n',file=fh)

    print('authors          : {}'.format(" ".join(authors)
                                               if authors else AUTHWARN),file=fh)


    print('included libs    : {}'.format(" ".join(included)),file=fh)
    print(code_size_report(code_metrics, {'lines': 25, 'words': 87}),file=fh)



    print("cpplint          : {}".format("{} problems".format(cpplint_count) if cpplint_count else "ok"),file=fh)
    for e in code_metrics['errors']:
        for x in code_metrics['errors'][e][:3]:
            print('line {} ({}): {}'.format(*x),file=fh)
    print("astyle           : {:.1%} code unchanged.\n".format(code_metrics['astyle']),file=fh)

    print('---- grading ----\n',file=fh)

    print('grading scheme',file=fh)
    print(' * 80 for passing the specifications.',file=fh)
    print(' * 10 points for astyle (% file unchanged by astyle)',file=fh)
    print(' * 10 points for cpplint, -2 deduction for each problem',file=fh)

    print('\nyour grades      :',Grade,file=fh)
    print('your grade total : {:.2f} / 100'.format(sum(Grade[x] for x in Grade)),file=fh)

    if save:
        return Grade,fh.getvalue()


def cppshell(Parms,q):
      vals = main_cpp(**Parms)
      q.put(vals)

if __name__ == '__main__':
    PD = {'source':"happy.cpp",'program':'happy','original':'happy.cpp'}
    T = run(['g++', "-std=c++14", PD['source'], "-o", PD['program']])
    if T.returncode:
        print(T)
        quit()
    main_cpp(PD['source'],PD['program'],PD['original'],faster_than_server=1)

    

