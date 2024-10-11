import os
import time
import shutil
import logging
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

# Налаштування журналу подій
logging.basicConfig(filename='backup_daemon.log', level=logging.INFO, format='%(asctime)s - %(message)s')

class BackupHandler(FileSystemEventHandler):
    def __init__(self, source_folder, backup_folder, max_backups):
        self.source_folder = source_folder
        self.backup_folder = backup_folder
        self.max_backups = max_backups

    def on_modified(self, event):
        self.create_backup(event.src_path)

    def on_created(self, event):
        self.create_backup(event.src_path)

    def create_backup(self, file_path):
        if os.path.isfile(file_path):
            relative_path = os.path.relpath(file_path, self.source_folder)
            backup_path = os.path.join(self.backup_folder, relative_path)
            backup_dir = os.path.dirname(backup_path)
            if not os.path.exists(backup_dir):
                os.makedirs(backup_dir)
            shutil.copy2(file_path, backup_path)
            logging.info(f'Backup created for {file_path}')
            self.manage_backups()

    def manage_backups(self):
        backups = sorted(os.listdir(self.backup_folder), key=lambda x: os.path.getmtime(os.path.join(self.backup_folder, x)))
        while len(backups) > self.max_backups:
            oldest_backup = backups.pop(0)
            os.remove(os.path.join(self.backup_folder, oldest_backup))
            logging.info(f'Old backup {oldest_backup} removed')

def daemonize():
    if os.fork() > 0:
        exit()
    os.setsid()
    if os.fork() > 0:
        exit()
    os.umask(0)
    os.chdir('/')
    with open('/dev/null', 'r') as dev_null:
        os.dup2(dev_null.fileno(), 0)
    with open('/dev/null', 'a+') as dev_null:
        os.dup2(dev_null.fileno(), 1)
        os.dup2(dev_null.fileno(), 2)

def main():
    source_folder = '/path/to/source_folder'
    backup_folder = '/path/to/backup_folder'
    max_backups = 5

    event_handler = BackupHandler(source_folder, backup_folder, max_backups)
    observer = Observer()
    observer.schedule(event_handler, path=source_folder, recursive=True)
    observer.start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()
    observer.join()

if __name__ == "__main__":
    daemonize()
    main()
