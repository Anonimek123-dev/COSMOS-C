# `buildenv/` 
There, we are building an environment to open our .iso system.

# Dockerfile
## Definition
A **Dockerfile** is a plain text file that acts as a **recipe for building a development environment**.

**You can think of it as step-by-step instructions that specify:**
1. **Which base system to use** (e.g., Linux, Ubuntu).
2. **Which programs or tools to install** (e.g., compiler, assembler, build utilities).
3. **How to configure** the working directory for the project.
4. **How to set up the environment** so that it can be reproduced anywhere.

**With Dockerfile:**
- **You don’t need to manually install tools** on your computer.
- **Everyone** using the same **Dockerfile** gets **the exact same environment.**
- **It eliminates** the classic **“works on my machine” problem.**
---

## Dockerfile in COSMOS-C
**Here is the Dockerfile in COSMOS-C:**
```dockerfile
FROM randomdude/gcc-cross-x86_64-elf

RUN apt-get update 
RUN apt-get upgrade -y
RUN apt-get install -y nasm
RUN apt-get install -y xorriso
RUN apt-get install -y grub-pc-bin
RUN apt-get install -y grub-common

VOLUME /root/env
WORKDIR /root/env
```