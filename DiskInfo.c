#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#define MAX_DRIVES 26

void PrintDriveInfo(const TCHAR *volumeName) {
    TCHAR label[MAX_PATH];
    TCHAR fileSystemName[MAX_PATH];
    DWORD serialNumber, maxComponentLen, fileSystemFlags;
    UINT driveType;
    TCHAR devicePath[MAX_PATH];
    TCHAR driveLetter[4];
    BOOL isMounted = FALSE;
    TCHAR physicalDiskPath[64];

    // Get the drive type
    driveType = GetDriveType(volumeName);

    // Get the volume label and file system name
    if (GetVolumeInformation(volumeName, label, MAX_PATH, &serialNumber, &maxComponentLen, &fileSystemFlags, fileSystemName, MAX_PATH)) {
        // Check if there's a drive letter associated
        if (_tcslen(volumeName) > 2) {
            _stprintf(driveLetter, _T("%c:"), volumeName[0]);
            isMounted = TRUE;
        } else {
            _tcscpy(driveLetter, _T("NONE ASSIGNED"));
        }
    } else {
        _tcscpy(label, _T("NO LABEL"));
        _tcscpy(fileSystemName, _T("UNKNOWN"));
        _tcscpy(driveLetter, _T("NONE ASSIGNED"));
    }

    // Determine the physical disk associated with the drive
    _stprintf(physicalDiskPath, _T("//./PhysicalDisk%d"), volumeName[0] - _T('A'));

    // Get the drive type (Fixed, Removable, etc.)
    TCHAR *typeString = _T("Unknown");
    switch (driveType) {
        case DRIVE_REMOVABLE:
            typeString = _T("Removable");
            break;
        case DRIVE_FIXED:
            typeString = _T("Fixed");
            break;
        case DRIVE_REMOTE:
            typeString = _T("Network");
            break;
        case DRIVE_CDROM:
            typeString = _T("CD-ROM");
            break;
        case DRIVE_RAMDISK:
            typeString = _T("RAM Disk");
            break;
    }

    // Print the details
    _tprintf(_T("%-15s\t%-20s\t%-10s\t%-10s\t%-10s\t%s\n"),
             label, physicalDiskPath, driveLetter, isMounted ? _T("YES") : _T("NO"), typeString, driveLetter);
}

int main() {
    TCHAR volumeName[MAX_PATH] = _T("A:\\");
    DWORD driveMask = GetLogicalDrives();
    if (driveMask == 0) {
        _tprintf(_T("Error retrieving drives: %d\n"), GetLastError());
        return 1;
    }

    _tprintf(_T("LABEL\t\tPhysical Disk\t\tDrive Letter\tMounted\tType\tMount Point\n"));
    _tprintf(_T("-------------------------------------------------------------------------------\n"));

    for (int i = 0; i < MAX_DRIVES; ++i) {
        if (driveMask & (1 << i)) {
            volumeName[0] = _T('A') + i;
            PrintDriveInfo(volumeName);
        }
    }

    return 0;
}
