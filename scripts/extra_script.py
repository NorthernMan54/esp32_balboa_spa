Import("env", "projenv")
import os

# Add the following line to your platformio.ini to enable this script
# extra_scripts = post:scripts/extra_script.py

# VITE_LOCAL_HOSTING=Enabled
#
# or
#
# VITE_HOSTING_URL=http://spaPanel-3C71BF9DFA90.local.

def before_buildfs(source, target, env):
    if not os.path.exists('balboa-spa'):
        print('Cloning balboa-spa')
        os.system('git clone https://github.com/jozefnad/balboa-spa.git')
        os.chdir('balboa-spa')
        print("Option A: VITE_LOCAL_HOSTING=Enabled")
        print("Option B: VITE_HOSTING_URL=http://" + env['UPLOAD_PORT'])
        print("\nBuilding with Option A")

        with open('.env', 'w') as file:
            file.write("#Choose one of the following options\n")
            file.write("VITE_LOCAL_HOSTING=Enabled\n")
            file.write("#VITE_HOSTING_URL=http://" + env['UPLOAD_PORT'])
        os.system('npm install')
        print('Building balboa-spa')
        os.system('npm run build')
        os.chdir('../')
    else:
        print('Building balboa-spa')
        os.chdir('balboa-spa')
        os.system('npm run build')
        os.chdir('../')

env.AddPreAction('$BUILD_DIR/littlefs.bin', before_buildfs)