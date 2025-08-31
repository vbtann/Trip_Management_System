# Trip Management System

A Qt-based desktop application for managing trips, people, and expenses with an intuitive graphical user interface.

## 📋 Table of Contents
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Configuration](#configuration)
- [Running the Application](#running-the-application)
- [Features](#features)
- [Troubleshooting](#troubleshooting)

## 🔧 Prerequisites

### Required Software
Before running this application, you must have **Qt Framework** installed on your machine.

#### Qt Installation Options:

1. **Qt Online Installer** (Recommended)
   - Download from: https://www.qt.io/download
   - Install Qt 5.15+ or Qt 6.x
   - Make sure to include MinGW compiler during installation

2. **Qt Creator with MinGW**
   - Ensure MinGW is included in your Qt installation
   - Typical installation path: `C:\Qt\Tools\mingw1310_64\bin` (version may vary)

## 🚀 Installation

1. **Clone or download** this repository to your local machine
2. **Navigate** to the project directory:
   ```
   cd Group_9/Project
   ```

## ⚙️ Configuration

### Important: Configure Build Script

Before running the application, you **MUST** modify the `build_run.bat` file to match your Qt installation path.

#### Steps:

1. **Open** `build_run.bat` in a text editor
2. **Locate** this line (around line 8):
   ```bat
   set MINGW_PATH=D:\Apps\Qt\Tools\mingw1310_64\bin
   ```
3. **Replace** the path with your actual Qt MinGW installation path

#### Common Qt Installation Paths:

```bat
:: For Qt installed in default location:
set MINGW_PATH=C:\Qt\Tools\mingw1310_64\bin

:: For Qt installed in custom location:
set MINGW_PATH=C:\YourCustomPath\Qt\Tools\mingw1310_64\bin

:: For different MinGW versions:
set MINGW_PATH=C:\Qt\Tools\mingw1120_64\bin
set MINGW_PATH=C:\Qt\Tools\mingw900_64\bin
```

#### How to Find Your MinGW Path:

1. **Open Qt Creator**
2. Go to **Tools** → **Options** → **Kits** → **Compilers**
3. Look for **MinGW** compiler entry
4. Copy the path (excluding the `/gcc.exe` part)
5. Use this path in the `MINGW_PATH` variable

**OR**

1. **Navigate** to your Qt installation directory
2. **Look for** the `Tools` folder
3. **Find** the `mingw` folder (e.g., `mingw1310_64`, `mingw1120_64`)
4. **Add** `\bin` to the end of the path

## 🏃‍♂️ Running the Application

### Method 1: Using Build Script (Recommended)

1. **Open Command Prompt** in the project directory
2. **Run** the build script:
   ```cmd
   build_run.bat
   ```
3. The script will:
   - Build the application
   - Deploy Qt dependencies
   - Copy necessary data files
   - Launch the application

### Method 2: Direct Execution (After First Build)

After successfully running the build script once, you can directly run:
```cmd
cd TripManagementSystem\release
TripManagementSystem.exe
```

## ✨ Features

- **People Management**: Add, edit, and manage people (hosts and members)
- **Trip Management**: Create and organize trips with detailed information
- **Expense Tracking**: Track and categorize trip-related expenses
- **Data Filtering**: Filter trips by date, host, status, and category
- **Persistent Storage**: Automatic data saving using JSON cache files
- **User-Friendly Interface**: Intuitive Qt-based GUI

## 🛠️ Troubleshooting

### Common Issues and Solutions

#### Issue: "Qt/qmake not found in PATH"
**Solution:**
1. Verify Qt is properly installed
2. Check if qmake is in your system PATH
3. Add Qt bin directory to PATH environment variable
4. Restart command prompt

#### Issue: "mingw32-make not found"
**Solution:**
1. Check your `MINGW_PATH` in `build_run.bat`
2. Verify MinGW is installed with Qt
3. Update the path to match your installation

#### Issue: Application doesn't start after double-clicking exe
**Solution:**
1. Run `build_run.bat` first to deploy dependencies
2. Ensure all Qt DLLs are in the release folder
3. Check if cache folder exists in release directory

#### Issue: "Missing Qt6Core.dll" or similar DLL errors
**Solution:**
1. Run the build script again
2. Ensure windeployqt worked properly
3. Manually copy missing DLLs from Qt bin directory

#### Issue: Build fails with compilation errors
**Solution:**
1. Verify Qt version compatibility (Qt 5.15+ or Qt 6.x)
2. Check if all source files are present
3. Clean build directory and rebuild

### Getting Help

If you encounter issues:

1. **Check** the console output for specific error messages
2. **Verify** all prerequisites are installed
3. **Ensure** paths in `build_run.bat` are correct
4. **Try** running from Qt Creator IDE as an alternative

### Project Structure
```
Project/
├── build_run.bat          # Main build and run script
├── TripManagementSystem/           # Application source code
│   ├── cache/             # Application data cache
│   ├── Managers/          # Business logic managers
│   ├── Models/            # Data models
│   ├── UI/                # User interface components
│   └── release/           # Built application (after build)
└── README.md              # This file
```

## 📝 Notes

- **First run**: Always use `build_run.bat` for the first execution
- **Subsequent runs**: You can use either the batch file or direct exe execution
- **Data persistence**: Application data is automatically saved to cache files
- **Updates**: Re-run `build_run.bat` after any code changes
- **Clean up**: Run the `clean.bat` to clean up all files that are created during the building process

---

**Developed by**: 
- Tran Viet Bao - 24127270
- Vu Tran Minh Hieu - 24127003
- Hoang Duc Thinh - 24127240