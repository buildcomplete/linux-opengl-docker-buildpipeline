FROM ubuntu:22.04

# Avoid prompts from apt
ENV DEBIAN_FRONTEND=noninteractive

# Install packages for OpenGL, textures, and basic development
RUN apt-get update && apt-get install -y \
    # OpenGL and Mesa utilities
    mesa-utils \
    mesa-utils-extra \
    libgl1-mesa-glx \
    libgl1-mesa-dri \
    # X11 libraries
    libx11-6 \
    libxext6 \
    libxrender1 \
    libxtst6 \
    libxi6 \
    # Texture and image libraries
    freeglut3-dev \
    libglew-dev \
    libdevil-dev \
    libsoil-dev \
    # Image utilities for testing
    imagemagick \
    # Basic development tools (minimal)
    gcc \
    g++ \
    make \
    pkg-config \
    # Utilities
    curl \
    wget \
    # Shell
    bash \
    bash-completion \
    # Additional packages for a better developer experience
    vim \
    nano \
    htop \
    git \
    && rm -rf /var/lib/apt/lists/*

# Create non-root user
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME \
    && apt-get update \
    && apt-get install -y sudo \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME \
    && chsh -s /bin/bash $USERNAME \
    && rm -rf /var/lib/apt/lists/*

# Switch to non-root user
USER $USERNAME
WORKDIR /workspace

# Set up environment for X11 forwarding
ENV DISPLAY=:0
RUN echo 'export DISPLAY=${DISPLAY:-:0}' >> ~/.bashrc

