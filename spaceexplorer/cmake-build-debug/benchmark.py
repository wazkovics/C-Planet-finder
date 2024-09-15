import subprocess
import threading
import sys
import math

if len(sys.argv) < 2:
    print("You must specify an executable path!\ne.g. python benchmark.py ./out.exe")
    sys.exit(1)
EXE_PATH = sys.argv[1]

NUM_RUNS = 5000
if len(sys.argv) > 2:
    NUM_RUNS = int(sys.argv[2])
CHUNK_SIZE = 8

def execute_one(seed: int) -> (int, bool):
    command = [EXE_PATH, str(seed)]
    try:
        result = subprocess.run(command, capture_output=True, text=True)
        success = "Congratulations" in result.stdout.strip().split('\n')[-1]
        hops = result.returncode
        if success:
            print(f"Ran with seed {seed}, {hops} hops")
            return (hops, False)
        else:
            print(f"Ran with seed {seed} [FAIL]")
            return (0, True)
    except subprocess.CalledProcessError as e:
        print(f"Error executing command with seed {seed}: {e}")
        sys.exit(1)

def execute_chunk(start: int, total_hops: list(), total_fails: list()):
    chunk_hops = 0
    chunk_fails = 0
    for seed in range(start, start + CHUNK_SIZE):
        hops, fail = execute_one(seed)
        chunk_hops += hops
        if fail:
            chunk_fails += 1
    total_hops[0] += chunk_hops
    total_fails[0] += chunk_fails

hops = [0]
fails = [0]

chunks = math.floor(NUM_RUNS / CHUNK_SIZE)
threads = []
for i in range(chunks):
    thread = threading.Thread(target=execute_chunk, args=(i * CHUNK_SIZE, hops, fails))
    threads.append(thread)
    thread.start()

for thread in threads:
    thread.join()

success_count = NUM_RUNS - fails[0]
success_rate = success_count / NUM_RUNS
fail_rate = fails[0] / NUM_RUNS
average_hops = hops[0] / success_count
print(f"Successes: {success_count}\nFailures: {fails[0]}\nSuccess Rate: {success_rate * 100:.2f}%\nFail Rate: {fail_rate * 100:.2f}%")
print(f"Average hops: {average_hops:.1f} ")