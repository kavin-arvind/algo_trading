import numpy as np
import csv
import os
import tarfile 
import argparse
import shutil
import subprocess
import multiprocessing
import time
import filecmp
from collections import defaultdict
import statistics


def remove_files_in_folder(folder_path, files_to_remove):
    for file_to_remove in files_to_remove:
        file_path = os.path.join(folder_path, file_to_remove)
        if os.path.exists(file_path):
            os.remove(file_path)

def copy_all_files(source_folder, destination_folder):
    # Ensure the source folder exists
    if not os.path.exists(source_folder) or not os.path.exists(destination_folder):
        print(f"Source folder '{source_folder}' or destination folder '{destination_folder}' does not exist.")
        return

    files_to_copy = [f for f in os.listdir(source_folder) if os.path.isfile(os.path.join(source_folder, f))]

    for file_name in files_to_copy:
        source_path = os.path.join(source_folder, file_name)
        destination_path = os.path.join(destination_folder, file_name)
        shutil.copy(source_path, destination_path)

# Define functions to run programs
def run_program(command,timeout):
    subprocess.run(command, shell=True, timeout=timeout, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

def run_phase_one(folder_path, part_num, path_to_input_file, timeout):
    # Run programs in parallel
    original_working_directory = os.getcwd()

    try:
        os.chdir(folder_path)
        program1_command = "./trader " +  str(part_num) +" > output.txt"
        program2_command = "./iml " + path_to_input_file

        process1 = multiprocessing.Process(target=run_program, args=(program1_command,timeout))
        process2 = multiprocessing.Process(target=run_program, args=(program2_command,timeout))

        process1.start()
        time.sleep(0.2)
        process2.start()

        process1.join()
        process2.join()

    finally:
        os.system("fuser -k 8888/tcp")
        os.chdir(original_working_directory)


def compile_phase_one(folder_path):
    original_working_directory = os.getcwd()
    try:
        os.chdir(folder_path)
        compilation_command = "make"
        compilation_result = subprocess.run(compilation_command, shell=True, stdout=subprocess.DEVNULL)

    finally:
        os.chdir(original_working_directory)

    if compilation_result.returncode != 0:
        return False
    else:
        return True
    
def run_phase_two_part_one(folder_path, num_threads, path_to_input_files, timeout):
    original_working_directory = os.getcwd()

    try:
        os.chdir(folder_path)
        copy_all_files(path_to_input_files, "../../../testcases/phase2/inputs/")

        program1_command = "./market " +  str(num_threads) +" > output.txt"

        run_program(program1_command,timeout)

        program2_command = "./MM > output_true.txt"

        run_program(program2_command,timeout)

    finally:
        os.chdir(original_working_directory)


def run_phase_two_part_two(folder_path, num_threads, path_to_input_files, timeout):
    original_working_directory = os.getcwd()

    try:
        os.chdir(folder_path)
        copy_all_files(path_to_input_files, "../../../testcases/phase2/inputs/")

        program1_command = "./trader " +  str(num_threads) +" > output.txt"

        process1 = multiprocessing.Process(target=run_program, args=(program1_command,timeout))

        process1.start()
        process1.join()        

    finally:
        os.system("./MM > output_true.txt")
        os.chdir(original_working_directory)


def compile_phase_two_part_one(folder_path):
    original_working_directory = os.getcwd()
    try:
        os.chdir(folder_path)
        compilation_command = "make market"
        compilation_result = subprocess.run(compilation_command, shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

    finally:
        os.chdir(original_working_directory)

    if compilation_result.returncode == 0:
        return True
    else:
        try:
            os.chdir(folder_path)
            compilation_command = "make altmarket"
            compilation_result = subprocess.run(compilation_command, shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

        finally:
            os.chdir(original_working_directory)
        
        if compilation_result.returncode != 0:
            return False
        else:
            return True
        
def compile_phase_two_part_two(folder_path):
    original_working_directory = os.getcwd()
    try:
        os.chdir(folder_path)
        compilation_command = "make trader"
        compilation_result = subprocess.run(compilation_command, shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

    finally:
        os.chdir(original_working_directory)

    if compilation_result.returncode == 0:
        return True
    else:
        try:
            os.chdir(folder_path)
            compilation_command = "make alttrader"
            compilation_result = subprocess.run(compilation_command, shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

        finally:
            os.chdir(original_working_directory)
        
        if compilation_result.returncode != 0:
            return False
        else:
            return True
    
def run_phase_three(folder_path, num_threads, path_to_input_files, timeout):
    # Run programs in parallel
    original_working_directory = os.getcwd()
    directory_path = "./outputs/"

    file_names = []
    for i in range(num_threads):
        file_names.append("output" + str(i+1) + ".txt")

    try:
        os.chdir(folder_path)
        copy_all_files(path_to_input_files, "../../../testcases/phase3/markets/")
        
        if not os.path.exists(directory_path):
            os.makedirs(directory_path)

        for file_name in file_names:
            file_path = os.path.join(directory_path, file_name)
            with open(file_path, 'w') as file:
                file.write("Output Trace File UID KarGo")
        
        program1_command = "./trader " +  str(num_threads) + " > outputs/output.txt"
        program2_command = "./iml " + str(num_threads)

        process1 = multiprocessing.Process(target=run_program, args=(program1_command,timeout))
        process2 = multiprocessing.Process(target=run_program, args=(program2_command,timeout))

        process1.start()
        time.sleep(0.2)
        process2.start()

        process1.join()
        process2.join()

    finally:
        os.system("fuser -k 8888/tcp")
        os.chdir(original_working_directory)


def compile_phase_three(folder_path):
    original_working_directory = os.getcwd()
    try:
        os.chdir(folder_path)
        compilation_command = "make"
        compilation_result = subprocess.run(compilation_command, shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    finally:
        os.chdir(original_working_directory)

    if compilation_result.returncode != 0:
        return False
    else:
        return True
    
def parse_line_p1p3(input_line):
    # Split the input line into individual elements
    elements = input_line.split()

    # Extract (name, quantity) pairs
    name_quantity_pairs = [(elements[i], int(elements[i + 1])) for i in range(0, len(elements) - 3, 2)]

    # Extract the (p, q, b) tuple
    p, q, b = elements[-3:]

    return name_quantity_pairs, (p, q, b)

class Team:
    def __init__(self, name1, name2) -> None:
        self.name_one = name1
        self.name_two = ""
        if name2 is None:
            self.num_members = 1
        else:
            self.name_two = name2
            self.num_members = 2


class SophieBatch:
    def __init__(self, args) -> None:
        self.teams = []
        self.all_usernames = []
        if args.full_batch:
            path_to_csv = "./ProjectTeams.csv"
        
            with open(path_to_csv, mode ='r') as file:
                csvFile = csv.reader(file)
                header = 1
                for line in csvFile:
                    if header:
                        header = 0
                        continue

                    r1 = line[4].strip().lower()
                    r2 = line[7].strip().lower()
                    if r1 in self.all_usernames or r2 in self.all_usernames:
                        print("Filled the form twice: ",r1,r2)

                    else:
                        if r2:
                            self.teams.append(Team(r1,r2))
                            self.all_usernames.append(r1)
                            self.all_usernames.append(r2)
                        else:
                            self.teams.append(Team(r1,None))
                            self.all_usernames.append(r1)

        else:
           name1 = args.rollno1
           name2 = args.rollno2
           self.teams = [Team(name1,name2)]
        
    
class GradingEnvironment:
    def __init__(self, args) -> None:
        self.batch = SophieBatch(args)

    def check_submissions(self):
        present = []
        names = []
        for team in self.batch.teams:
            if team.num_members == 1:
                file_exists = os.path.exists("./submissions/" + team.name_one)
                if file_exists:
                    names.append(team.name_one)
                else:
                    names.append("")
            else:
                file_exists = os.path.exists("./submissions/" + team.name_one + "_" + team.name_two + ".tar.gz") or os.path.exists("./submissions/" + team.name_two + "_" + team.name_one + ".tar.gz")
                if file_exists and os.path.exists("./submissions/" + team.name_one + "_" + team.name_two + ".tar.gz"):
                    names.append(team.name_one + "_" + team.name_two)
                elif file_exists:
                    names.append(team.name_two + "_" + team.name_one)
                else:
                    names.append("")
            present.append(file_exists)
        return present, names
    
    def check_final_submissions(self):
        present = []
        names = []
        for team in self.batch.teams:
            if team.num_members == 1:
                file_exists = os.path.exists("./final_submissions/" + team.name_one)
                if file_exists:
                    names.append(team.name_one)
                else:
                    names.append("")
            else:
                file_exists = os.path.exists("./final_submissions/" + team.name_one + "_" + team.name_two + ".tar.gz") or os.path.exists("./final_submissions/" + team.name_two + "_" + team.name_one + ".tar.gz")
                if file_exists and os.path.exists("./final_submissions/" + team.name_one + "_" + team.name_two + ".tar.gz"):
                    names.append(team.name_one + "_" + team.name_two)
                elif file_exists:
                    names.append(team.name_two + "_" + team.name_one)
                else:
                    names.append("")
            present.append(file_exists)
        return present, names
        
    
class Autograder:
    def __init__(self, args) -> None:
        self.args = args
        self.grading_environment = GradingEnvironment(args)
        self.mark_list_p1p1 = [0 for i in self.grading_environment.batch.teams]
        self.mark_list_p1p2 = [0 for i in self.grading_environment.batch.teams]
        self.mark_list_p1p3 = [0 for i in self.grading_environment.batch.teams]
        self.mark_list_p2p1 = [0 for i in self.grading_environment.batch.teams]
        self.mark_list_p2p2 = [0 for i in self.grading_environment.batch.teams]
        self.mark_list_p3 = [0 for i in self.grading_environment.batch.teams]
        self.comments = ["" for i in self.grading_environment.batch.teams]
        self.p1p1_testcases = ["input_Jumbled.txt", "input_long.txt", "input_public.txt", "input2.txt", "input3.txt", "input4.txt"]
        self.p1p1_solutions = ["output_Jumbled.txt", "output_long.txt", "output_public.txt","output2.txt", "output3.txt", "output4.txt"]
        self.p1p1_timeouts = [1,1,1,1,1,1]
        self.p1p1_marks = [1,1,1,1,1,1]
        self.p1p2_testcases = ["input1.txt","input2.txt","input3.txt","input4.txt","input5.txt"]
        self.p1p2_solutions = ["output1.txt","output2.txt","output3.txt","output4.txt", "output5.txt"]
        self.p1p2_timeouts = [1,1,1,2,2]
        self.p1p2_marks = [1,1,1,1,1]
        self.p1p3_testcases = ["input1.txt", "input1_alt.txt","o_input2.txt","o_input3.txt","o_input4.txt","o_input5.txt","o_input6.txt","o_input7.txt","o_input8.txt","o_input9.txt","o_input10.txt","o_input11.txt","o_input100.txt"]
        self.p1p3_solutions = ["output1.txt", "output1_alt.txt", "o_output2.txt", "o_output3.txt", "o_output4.txt", "o_output5.txt", "o_output6.txt", "o_output7.txt", "o_output8.txt", "o_output9.txt", "o_output10.txt", "o_output11.txt", "o_output100.txt"]
        self.p1p3_timeouts = [1,1,30,5,5,5,5,5,5,5,5,5,5]
        self.p1p3_marks = [1,1,2,1,1,1,1,1,1,1,1,2,1]
        self.p2p1_testcases = ["inputs1/","inputs2/","inputs3/","inputs4/","inputs5/"]
        self.p2p1_solutions = []
        self.p2p1_timeouts = [10,20,30,40,50]
        self.p2p1_marks = [1,1,1,1,1]
        self.p2p2_testcases = ["inputs1/","inputs2/","inputs3/","inputs4/","inputs5/","inputs6/"]
        self.p2p2_solutions = []
        self.p2p2_timeouts = [20,40,60,80,100,10]
        self.p2p2_marks = [1,1]
        self.p2_num_threads = [2,5,10,25,50]
        self.p3_testcases = ["inputs1/","inputs2/"]
        self.p3_num_threads = [2,3]
        self.p3_solutions = ["outputs1/","outputs2/"]
        self.p3_timeouts = [5,15]
        self.p3_marks = [1,4]
        
    def untar_submission(self, team_name):
        if os.path.exists('./submissions/'+ team_name):
            return True 

        try:
            file = tarfile.open('./submissions/' + team_name + ".tar.gz")
            file.extractall('./submissions/') 
            file.close()
        except Exception as e:
            print(e)
            return False

        file_exists = os.path.exists("./submissions/" + team_name)
        if file_exists:
            return True
        else:
            return False
        
    def untar_final_submission(self, team_name):
        if os.path.exists('./final_submissions/'+ team_name):
            return True 

        try:
            file = tarfile.open('./final_submissions/' + team_name + ".tar.gz")
            file.extractall('./final_submissions/') 
            file.close()
        except:
            return False

        file_exists = os.path.exists("./final_submissions/" + team_name)
        if file_exists:
            return True
        else:
            return False
        
    def setup_phase_one(self, idx, team_name):
        destination_folder = "./submissions/" + team_name + "/phase1/"
        source_folder = "./files/phase1/"
        remove_files_in_folder("./submissions/" + team_name + "/phase1/", ["Makefile","receiver.h","iml.cpp"])
        copy_all_files(source_folder, destination_folder)
        
    def verify_phase_one_part_one(self, target_file, output_file):
        with open(target_file) as file:
            lines_target = [line.rstrip() for line in file]

        with open(output_file) as file:
            lines_output = [line.rstrip() for line in file]

        for id, line in enumerate(lines_target):
            if id >= len(lines_output) or line not in [lines_output[id]]:
                return False

        return True

    def verify_phase_one_part_two(self, target_file, output_file):
        with open(target_file) as file:
            lines_target = [line.rstrip() for line in file]

        with open(output_file) as file:
            lines_output = [line.rstrip() for line in file]

        for id, line in enumerate(lines_target):
            if id >= len(lines_output):
                return False
            out_line = lines_output[id]
            if out_line == line:
                continue
            else:
                o1 = [line.split(' ')[n:n+2] for n in range(0, len(line.split(' ')), 2)]
                o2 = [out_line.split(' ')[n:n+2] for n in range(0, len(out_line.split(' ')), 2)]
                if len(o1) != len(o2):
                    return False
                else:
                    for i in o1:
                        if i not in o2:
                            return False
                        
        
        return True
    
    def verify_phase_one_part_three(self, target_file, output_file):
        with open(target_file) as file:
            lines_target = [line.rstrip() for line in file]

        with open(output_file) as file:
            lines_output = [line.rstrip() for line in file]

        reduced_correct_lines = []
        reduced_actual_lines = []

        for line in lines_target:
            if not line:
                continue
            if line == "Receiver is listening on port 8888" or line == "No Trade":
                reduced_correct_lines.append(line)
                continue
            elif line.isdigit():
                reduced_correct_lines.append(line)
                continue
            else:
                structure, meta = parse_line_p1p3(line)
                structure.append((int(meta[0]),meta[2]))
                for i in range(int(meta[1])):
                    reduced_correct_lines.append(structure)

        for line in lines_output:
            if not line:
                continue
            if line == "Receiver is listening on port 8888" or line == "No Trade":
                reduced_actual_lines.append(line)
                continue
            elif line.isdigit():
                reduced_actual_lines.append(line)
                continue
            else:
                structure, meta = parse_line_p1p3(line)
                structure.append((int(meta[0]),meta[2]))
                for i in range(int(meta[1])):
                    reduced_actual_lines.append(structure)

        if reduced_actual_lines == reduced_correct_lines:
            return True
        else:
            return False

    def grade_phase_one_part_one(self, idx, team_name):
        folder = "./submissions/" + team_name + "/phase1/"
    
        for id, path in enumerate(self.p1p1_testcases):
            try:
                run_phase_one(folder,1,"../../../testcases/phase1/part1/" + path,self.p1p1_timeouts[id])
                if self.verify_phase_one_part_one("solutions/phase1/part1/" + self.p1p1_solutions[id],folder+"output.txt"):
                    self.mark_list_p1p1[idx] += self.p1p1_marks[id]
                else:
                    print("Phase 1 Part 1",path)
            except:
                continue
                
        
    def grade_phase_one_part_two(self, idx, team_name):
        folder = "./submissions/" + team_name + "/phase1/"
    
        for id, path in enumerate(self.p1p2_testcases):
            try:
                run_phase_one(folder,2,"../../../testcases/phase1/part2/" + path,self.p1p2_timeouts[id])
                if self.verify_phase_one_part_two("solutions/phase1/part2/" + self.p1p2_solutions[id],folder+"output.txt"):
                    self.mark_list_p1p2[idx] += self.p1p2_marks[id]
                else:
                    print("Phase 1 Part 2",path)
            except:
                continue
    

    def grade_phase_one_part_three(self, idx, team_name):
        folder = "./submissions/" + team_name + "/phase1/"
    
        for id, path in enumerate(self.p1p3_testcases):
            try:
                run_phase_one(folder,3,"../../../testcases/phase1/part3/" + path,self.p1p3_timeouts[id])
                if self.verify_phase_one_part_three("solutions/phase1/part3/" + self.p1p3_solutions[id],folder+"output.txt"):
                    self.mark_list_p1p3[idx] += self.p1p3_marks[id]
                else:
                    print("Phase 1 Part 3",path)
            except:
                continue
        
    def grade_phase_one(self, idx, team_name):
        if not os.path.exists("./submissions/" + team_name + "/phase1/"):
            self.comments[idx] += ",Phase 1 Submission not Found"
            return False
        else:
            self.setup_phase_one(idx,team_name)

            folder = "./submissions/" + team_name + "/phase1/"
            if not compile_phase_one(folder):
                self.comments[idx] += ".Compilation Error For Phase 1"
                self.mark_list_p1p1[idx] = 0
                self.mark_list_p1p2[idx] = 0
                self.mark_list_p1p3[idx] = 0
                return

            self.grade_phase_one_part_one(idx, team_name)
            self.grade_phase_one_part_two(idx, team_name)
            self.grade_phase_one_part_three(idx, team_name)

    def setup_phase_two(self, idx, team_name):
        destination_folder = "./final_submissions/" + team_name + "/phase2/"
        source_folder = "./files/phase2/"
        remove_files_in_folder("./final_submissions/" + team_name + "/phase2/", ["Makefile","main.cpp","worker.cpp"])
        copy_all_files(source_folder, destination_folder)

    
    def verify_phase_two_part_one(self, target_file, output_file): 
        with open(target_file) as file:
            lines_target = [line.rstrip() for line in file]

        with open(output_file) as file:
            lines_output = [line.rstrip() for line in file]

        reduced_correct_lines = []
        reduced_actual_lines = []

        flag = False

        for line in lines_target:
            if not line:
                continue
            if line.split(' ')[1] == "purchased" and line.split(' ')[2] != '0':
                reduced_correct_lines.append(line)
            else:
                continue

        for line in lines_output:
            if not line or line == "TL" or line == "!@" or line == "@!":
                continue
            if line.split(' ')[1] == "purchased" and line.split(' ')[2] != '0':
                reduced_actual_lines.append(line)
            else:
                continue

        if reduced_actual_lines == reduced_correct_lines:
            return True
        else:
            return False

    def grade_phase_two_part_one(self, idx, team_name):
        folder = "./final_submissions/" + team_name + "/phase2/"

        if not compile_phase_two_part_one(folder):
            self.comments[idx] += ".Compilation Error For Phase 2 Part 1"
            self.mark_list_p2p1[idx] = 0
            return
        
        for id, path in enumerate(self.p2p1_testcases):
            try:
                run_phase_two_part_one(folder,self.p2_num_threads[id], "../../../testcases/phase2/" + path,self.p2p1_timeouts[id])
                if self.verify_phase_two_part_one(folder+"output_true.txt",folder+"output.txt"):
                    self.mark_list_p2p1[idx] += self.p2p1_marks[id]
                else:
                    print("Phase 2 Part 1",path)
            except Exception as e:
                print("Error",e)
                continue

    def verify_phase_two_part_two(self, output): 
        with open(output) as file:
            lines = [line.rstrip() for line in file]
        
        flag = False
        truncated_lines = []
        for line in lines:
            if not line:
                continue
            if line == "Successfully Initiated!":
                flag = True
                continue

            if flag:
                if line[0] == '-':
                    break

                truncated_lines.append(line)

        share_prices = defaultdict(list)

        for transaction in truncated_lines:
            parts = transaction.split()
            shares = int(parts[2])
            share_type = parts[5:-4]
            price_per_share = int(parts[-2].split('/')[0][1:])  # Extracting the price from the transaction

            total_cost = shares * price_per_share
            share_prices[share_type].append(total_cost)

        median_prices = {share_type: statistics.median(prices) for share_type, prices in share_prices.items()}
        
        profit = 0
        tradeMarker = 0
        for transaction in truncated_lines:
            parts = transaction.split()
            shares = int(parts[2])
            share_type = parts[5:-4]
            price_per_share = int(parts[-2].split('/')[0][1:])  # Extracting the price from the transaction
            isBuy = parts[0][0].isdigit()
            isSell = parts[-3][0].isdigit()

            if isBuy or isSell:
                tradeMarker = 1

            if isSell:
                profit += (price_per_share - median_prices[share_type]) * shares
            if isBuy:
                profit -= (price_per_share - median_prices[share_type]) * shares


        return tradeMarker, profit
            

    def grade_phase_two_part_two(self, idx, team_name):
        folder = "./final_submissions/" + team_name + "/phase2/"

        if not compile_phase_two_part_two(folder):
            self.comments[idx] += ".Compilation Error For Phase 2 Part 2"
            self.mark_list_p2p2[idx] = 0
            return
        
        for id, path in enumerate(self.p2p2_testcases):
            try:
                run_phase_two_part_two(folder,self.p2_num_threads[id], "../../../testcases/phase2/" + path,self.p2p2_timeouts[id])
                marker, profit = self.verify_phase_two_part_two(folder+"output.txt")
                if marker:
                    self.mark_list_p2p2[idx] += profit
                else:
                    print("Phase 2 Part 2",path)
            except:
                continue

    def grade_phase_two(self, idx, team_name):
        if not os.path.exists("./final_submissions/" + team_name + "/phase2"):
            self.comments[idx] += ",Phase 2 Submission not Found"
            return False
        else:
            self.setup_phase_two(idx, team_name)
            self.grade_phase_two_part_one(idx, team_name)
            self.grade_phase_two_part_two(idx, team_name)

    def setup_phase_three(self, idx, team_name):
        destination_folder = "./final_submissions/" + team_name + "/phase3/"
        source_folder = "./files/phase3/"
        remove_files_in_folder("./final_submissions/" + team_name + "/phase3/", ["Makefile","iml.cpp"])
        copy_all_files(source_folder, destination_folder)

    def verify_phase_three(self, target_output_folder, actual_output_folder):
        files_folder1 = [f for f in os.listdir(target_output_folder) if os.path.isfile(os.path.join(target_output_folder, f))]
        files_folder2 = [f for f in os.listdir(actual_output_folder) if os.path.isfile(os.path.join(actual_output_folder, f))]

        # Check if files in folder1 have corresponding files with the same name and content in folder2
        for file_name in files_folder1:
            file_path1 = os.path.join(target_output_folder, file_name)
            file_path2 = os.path.join(actual_output_folder, file_name)

            if file_name not in files_folder2 or not filecmp.cmp(file_path1, file_path2):
                return False
            
        return True

    def grade_phase_three(self, idx, team_name):
        if not os.path.exists("./final_submissions/" + team_name + "/phase3"):
            self.comments[idx] += ",Phase 3 Submission not Found"
            return False
        else:
            self.setup_phase_three(idx, team_name)
            folder = "./final_submissions/" + team_name + "/phase3/"

            if not compile_phase_three(folder):
                self.comments[idx] += ",Compilation Error for Phase 3"
                self.mark_list_p3[idx] = 0
                return
    
            for id, path in enumerate(self.p3_testcases):
                try:
                    run_phase_three(folder,self.p3_num_threads[id],"../../../testcases/phase3/" + path,self.p3_timeouts[id])
                    if self.verify_phase_three("solutions/phase3/" + self.p3_solutions[id],folder+"outputs/"):
                        self.mark_list_p3[idx] += self.p3_marks[id]
                    else:
                        print("Phase 3",path)
                except:
                    continue


    def grade_submission(self, idx, team_name):
        valid_submission = self.untar_submission(team_name)
        if not valid_submission:
            self.comments[idx] += ",Invalid submission 1 format"
            self.mark_list_p1p1[idx] = 0
            self.mark_list_p1p2[idx] = 0
            self.mark_list_p1p3[idx] = 0
        else:
            if self.args.p1:
                print("Grading phase 1",team_name)
                self.grade_phase_one(idx, team_name)


    def grade_final_submission(self, idx, team_name):
        valid_submission = self.untar_final_submission(team_name)
        if not valid_submission:
            self.comments[idx] += ",Invalid final submission format"
            self.mark_list_p2p1[idx] = 0
            self.mark_list_p2p2[idx] = 0
            self.mark_list_p3[idx] = 0
        else:
            if self.args.p2:
                print("Grading phase 2",team_name)
                self.grade_phase_two(idx, team_name)
            if self.args.p3:
                print("Grading phase 3",team_name)
                self.grade_phase_three(idx, team_name)

    def grade(self):
        submission_present_array, team_names = self.grading_environment.check_submissions()

        for idx, i in enumerate(submission_present_array):
            if i:
                self.grade_submission(idx, team_names[idx])
            else:
                self.mark_list_p1p1[idx] = 0
                self.mark_list_p1p2[idx] = 0
                self.mark_list_p1p3[idx] = 0
                self.comments[idx] += ",Submission for Phase 1 Not Found"


        submission_present_array, team_names = self.grading_environment.check_final_submissions()

        for idx, i in enumerate(submission_present_array):
            if i:
                self.grade_final_submission(idx, team_names[idx])
            else:
                self.mark_list_p2p1[idx] = 0
                self.mark_list_p2p2[idx] = 0
                self.mark_list_p3[idx] = 0
                self.comments[idx] += ",Final Submission Not Found"


    def show_grades(self):
        fields = ['Username', 'P1P1', 'P1P2', 'P1P3', 'P2P1','P2P2','P3','Comments'] 
        filename = "grades.csv"

        rows = []
        for idx, team in enumerate(self.grading_environment.batch.teams):
            rows.append([team.name_one, self.mark_list_p1p1[idx], self.mark_list_p1p2[idx],self.mark_list_p1p3[idx],
                         self.mark_list_p2p1[idx],self.mark_list_p2p2[idx],self.mark_list_p3[idx], self.comments[idx]])
            if team.num_members == 2:
                rows.append([team.name_two, self.mark_list_p1p1[idx], self.mark_list_p1p2[idx],self.mark_list_p1p3[idx],
                             self.mark_list_p2p1[idx],self.mark_list_p2p2[idx],self.mark_list_p3[idx], self.comments[idx]])

        rows = sorted(rows, key=lambda x: x[0])

        with open(filename, 'w') as csvfile:  
            csvwriter = csv.writer(csvfile)  
            csvwriter.writerow(fields)  
            csvwriter.writerows(rows) 


ap = argparse.ArgumentParser()
ap.add_argument("--rollno1",                   type=str)
ap.add_argument("--rollno2",                   type=str)
ap.add_argument("--full_batch",                action='store_true')
ap.add_argument("--p1",                action='store_true')
ap.add_argument("--p2",                action='store_true')
ap.add_argument("--p3",                action='store_true')
av = ap.parse_args()


grader = Autograder(av)
grader.grade()
grader.show_grades()