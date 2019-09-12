"""checker for poem

Note: the author emails are not validated against the actual student list.
"""

COMMENT_STRING = {'py': '#', 'sh': "#", 'cpp': '//'}

AUTHWARN = "WARNING, NO VALID AUTHOR LINES FOUND"

from subprocess import PIPE,Popen,run
import re
import sys
import cpplint
import difflib
from io import StringIO

program = 'poem'
program_name = program+'.cpp'


ASTYLE_OPTIONS = [
    '--style=google', '--indent=spaces=2', '--formatted', '--dry-run'
]

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
        category,subcategory = lint_type.split('/')
        if category not in Errors:
            Errors[category]=[]
        Errors[category].append( (line_number,lint_type,message) )

    lines = read_file_for_cpplint(program_filename)
    cpplint.RemoveMultiLineComments(program_filename,lines,error_fcn)

    clean_lines = cpplint.CleansedLines(lines)

    cpplint.ProcessFileData(program_filename,'cpp',lines,error_fcn)

    #for e in Errors:
    #    print(e,len(Errors[e]))
    #    for x in Errors[e][:3]:
    #        print('line {} ({}): {}'.format(*x))

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

def poem_tester(program_to_run):
    try:
        R = run(['./'+program_to_run], stdout=PIPE)
        lines = R.stdout.decode().splitlines()
        if len(lines) != 3:
            return False,'Number of poetry lines incorrect: {}'.format(len(lines))

        if not (len(lines[0]) < len(lines[1]) < len(lines[2])):
            return False,'Your poem is not the correct shape'

    except Exception as e:
        print(e)
        return False,'Program caused checker to crash.'

    return True,"Good triangle poem."


def main_cpp(source_file,program_to_run,original_name,faster_than_server=1,save=False):
    fh = StringIO() if save else sys.stdout

    print('Checking {} for EC327 submission.\n'.format(original_name),file=fh)

    Grade={'specs':0}

    the_program = read_file(source_file)
    authors = get_authors(the_program, 'cpp')
    included = get_includes(the_program)
    

    #run the specification tests
    print('---- testing your program against the specifications ----',file=fh)
    passed, report = poem_tester(program_to_run)

    print(file=fh)
    print(report,file=fh)
    print(file=fh)

    code_metrics = code_analysis_cpp(source_file)

    if code_metrics['astyle']=="error":
        print('astyle is reporting a problem.',file=fh)
        code_metrics['astyle']=0

    D = code_metrics['errors']
    cpplint_count= sum(len(D[x]) for x in D)
    

    Grade['specs'] = 80 * passed

    Grade['cpplint'] = max(0, 10-2*cpplint_count)
    Grade['astyle'] = 10*code_metrics['astyle']


    print('---- analysis of your code structure ----\n',file=fh)

    print('authors          : {}'.format(" ".join(authors)
                                               if authors else AUTHWARN),file=fh)


    print('included libs    : {}'.format(" ".join(included)),file=fh)
    #print(code_size_report(code_metrics, {'lines': 91, 'words': 332}),file=fh)



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
    PD = {'source':"poem.cpp",'program':'poem','original':'poem.cpp'}
    T = run(['g++', "-std=c++14", PD['source'], "-o", PD['program']])
    if T.returncode:
        print(T)
        quit()
    main_cpp(PD['source'],PD['program'],PD['original'],faster_than_server=1)

    

