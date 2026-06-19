import re

syms = set()
with open('make_errors.log') as f:
    for line in f:
        if 'multiple definition of' in line:
            try:
                # The line format: multiple definition of `function_name(args...)'; or `variable_name'
                sym = line.split('`')[1].split("'")[0].split('(')[0]
                syms.add(sym)
            except:
                pass

# The ones that MUST be shared
shared = {
    'bb', 'screen_w', 'screen_h', 'pitch',
    'mouse_x', 'mouse_y', 'mouse_down', 'mouse_just_pressed',
    'linux_scancode', 'linux_mx', 'linux_my', 'linux_mdown',
    'play_linux_freq', 'play_linux_sound', 'linux_wav_buffer',
    'run_smash_cats_engine', 'cartridge_start', 'cartridge_end'
}

with open('defines.txt', 'w') as out:
    for sym in sorted(syms):
        if sym not in shared:
            out.write(f"#define {sym} os2_{sym}\n")

print(f"Generated {len(syms) - len(shared.intersection(syms))} #defines.")
