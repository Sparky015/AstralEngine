# Software Requirements Specification
## For Ayla Engine
Version 1.0.0  
Ayla Engine
Jun 18, 2024, N/A 
Prepared by Andrew Fagan

Table of Contents
=================
* [Revision History](#revision-history)
* [Introduction](#introduction)
  * 1.1 [Purpose](#11-purpose)
  * 1.2 [Document Conventions](#12-document-conventions)
  * 1.3 [Intended Audience and Reading Suggestions](#13-intended-audience-and-reading-suggestions)
  * 1.4 [Product Scope](#14-product-scope)
  * 1.5 [References](#15-references)
* [Overall Description](#overall-description)
  * 2.1 [Product Perspective](#21-product-perspective)
  * 2.2 [Product Functions](#22-product-functions)
  * 2.3 [User Classes and Characteristics](#23-user-classes-and-characteristics)
  * 2.4 [Operating Environment](#24-operating-environment)
  * 2.5 [Design and Implementation Constraints](#25-design-and-implementation-constraints)
  * 2.6 [User Documentation](#26-user-documentation)
  * 2.7 [Assumptions and Dependencies](#27-assumptions-and-dependencies)
* [External Interface Requirements](#external-interface-requirements)
  * 3.1 [User Interfaces](#31-user-interfaces)
  * 3.2 [Hardware Interfaces](#32-hardware-interfaces)
  * 3.3 [Software Interfaces](#33-software-interfaces)
  * 3.4 [Communications Interfaces](#34-communications-interfaces)
* [System Features](#system-features)
  * 4.1 [System Feature 1](#41-system-feature-1)
  * 4.2 [System Feature 2 (and so on)](#42-system-feature-2-and-so-on)
* [Other Nonfunctional Requirements](#other-nonfunctional-requirements)
  * 5.1 [Performance Requirements](#51-performance-requirements)
  * 5.2 [Safety Requirements](#52-safety-requirements)
  * 5.3 [Security Requirements](#53-security-requirements)
  * 5.4 [Software Quality Attributes](#54-software-quality-attributes)
  * 5.5 [Business Rules](#55-business-rules)
* [Other Requirements](#other-requirements)
* [Appendix A: Glossary](#appendix-a-glossary)
* [Appendix B: Analysis Models](#appendix-b-analysis-models)
* [Appendix C: To Be Determined List](#appendix-c-to-be-determined-list)




## Revision History
| Name | Date    | Reason For Changes  | Version   |
| ---- | ------- | ------------------- | --------- |
|      |         |                     |           |

## Introduction

### 1.1 Purpose
> Identify the product whose software requirements are specified in this document, including the revision or release
> number. Describe the scope of the product that is covered by this SRS, particularly if this SRS describes only part
> of the system or a single subsystem.

This document covers the game engine as a whole which will be serving as an overview of the engine.


### 1.2 Document Conventions
> Describe any standards or typographical conventions that were followed when writing this SRS, such as fonts or
> highlighting that have special significance. For example, state whether priorities  for higher-level requirements are
> assumed to be inherited by detailed requirements, or whether every requirement statement is to have its own priority.

N/A?


### 1.3 Intended Audience and Reading Suggestions
> Describe the different types of reader that the document is intended for, such as developers, project managers,
> marketing staff, users, testers, and documentation writers. Describe what the rest of this SRS contains and how it
> is organized. Suggest a sequence for reading the document, beginning with the overview sections and proceeding through
> the sections that are most pertinent to each reader type.

This is just for my organization, so developer, user, tester... all of the above really. Its just me.


### 1.4 Product Scope
> Provide a short description of the software being specified and its purpose, including relevant benefits, objectives,
> and goals. Relate the software to corporate goals or business strategies. If a separate vision and scope document is
> available, refer to it rather than duplicating its contents here.

The scope of this SRS will include the game engine as a whole and the sub systems that will make the engine up from a
technically abstracted. The goal of this project is to not only provide me an experience in which I can learn and 
research about all sorts of different technologies involved with making a game engine, but allow me to foster and 
develop my passions in the space to help find what path I want to take for my future.


### 1.5 References
> List any other documents or Web addresses to which this SRS refers. These may include user interface style guides,
> contracts, standards, system requirements specifications, use case documents, or a vision and scope document. Provide
> enough information so that the reader could access a copy of each reference, including title, author, version number,
> date, and source or location.

N/A


## Overall Description

### 2.1 Product Perspective
> Describe the context and origin of the product being specified in this SRS. For example, state whether this product
> is a follow-on member of a product family, a replacement for certain existing systems, or a new, self-contained
> product. If the SRS defines a component of a larger system, relate the requirements of the larger system to the
> functionality of this software and identify interfaces between the two. A simple diagram that shows the major
> components of the overall system, subsystem interconnections, and external interfaces can be helpful.

The project will be a new game engine that is not from any existing system. This is the start of a game engine that is 
coded by me with influence from the material I use to learn about game engines and components of game engines.

### 2.2 Product Functions
> Summarize the major functions the product must perform or must let the user perform. Details will be provided in
> Section 3, so only a high level summary (such as a bullet list) is needed here. Organize the functions to make them
> understandable to any reader of the SRS. A picture of the major groups of related requirements and how they relate,
> such as a top level data flow diagram or object class diagram, is often effective.

Must be able to:
* Run a 2D & 3D game
* Create Games in a Level Editor
* Run without performance issues


Needs to have:
* An audio system
* A renderer
* A level editor
* A component system
* A memory management system
* Profilers
* Graphs and Data of Performance
* more to write later

### 2.3 User Classes and Characteristics
> Identify the various user classes that you anticipate will use this product. User classes may be differentiated based
> on frequency of use, subset of product functions used, technical expertise, security or privilege levels, educational
> level, or experience. Describe the pertinent characteristics of each user class. Certain requirements may pertain only
> to certain user classes. Distinguish the most important user classes for this product from those who are less important
> to satisfy.

To be used by me.


### 2.4 Operating Environment
> Describe the environment in which the software will operate, including the hardware platform, operating system and
> versions, and any other software components or applications with which it must peacefully coexist.

The engine will be available to use on MacOS and Windows for only the latest versions available respectively.

### 2.5 Design and Implementation Constraints
> Describe any items or issues that will limit the options available to the developers. These might include: corporate
> or regulatory policies; hardware limitations (timing requirements, memory requirements); interfaces to other
> applications; specific technologies, tools, and databases to be used; parallel operations; language requirements;
> communications protocols; security considerations; design conventions or programming standards (for example, if the
> customer’s organization will be responsible for maintaining the delivered software).

The main limitation is the amount of knowledge I have on game engines, which this project serves to help me in.


### 2.6 User Documentation
> List the user documentation components (such as user manuals, on-line help, and tutorials) that will be delivered
> along with the software. Identify any known user documentation delivery formats or standards.

At this time, there will only be documentation for each of the systems as well as their classes and functions.

### 2.7 Assumptions and Dependencies
> List any assumed factors (as opposed to known facts) that could affect the requirements stated in the SRS. These
> could include third-party or commercial components that you plan to use, issues around the development or operating
> environment, or constraints. The project could be affected if these assumptions are incorrect, are not shared, or
> change. Also identify any dependencies the project has on external factors, such as software components that you
> intend to reuse from another project, unless they are already documented elsewhere (for example, in the vision and
> scope document or the project plan).

I plan to depend on a number of things to allow for cross development and cross compatibility. Firstly, I use
CMake for building for the engine through CLion's build tools. I use GLFW to support cross-platform windows. Additionally,
I use OpenGL for cross-platform rendering. In the future, I plan to support platform dependent components like Metal,
DX11, and DX12 for rendering and Win32 and Cocoa for the window system.


## External Interface Requirements

### 3.1 User Interfaces
> Describe the logical characteristics of each interface between the software product and the users. This may include
> sample screen images, any GUI standards or product family style guides that are to be followed, screen layout
> constraints, standard buttons and functions (e.g., help) that will appear on every screen, keyboard shortcuts, error
> message display standards, and so on. Define the software components for which a user interface is needed. Details of
> the user interface design should be documented in a separate user interface specification.

N/A

TBD for level editor

### 3.2 Hardware Interfaces
> Describe the logical and physical characteristics of each interface between the software product and the hardware
> components of the system. This may include the supported device types, the nature of the data and control interactions
> between the software and the hardware, and communication protocols to be used.

Logical Characteristics:

The game engine sends draw calls to the GPU to render models, textures, and effects based on the game state.
Game logic such as physics simulations, AI, or game rules are computed in the CPU.
Inputs from peripherals like keyboard or controller are routed through the OS into the game engine to affect the game state.

Physical Characteristics:

The engine interacts directly with the GPU to run shaders and render graphics.
It uses the CPU for running game logic and other tasks.
It communicates with memory to store and retrieve game state and assets.

Supported Device Types:

The engine should support a variety of GPUs and CPUs for different performance levels and manufacturers.
It must also accept input from common peripherals such as keyboards, mice, game controllers, and potentially VR headsets, depending on the type of games it's designed to create.

Nature of the data and control interactions:

Continuous streams of positional data, textures, and commands are sent to the GPU.
User inputs and commands are interpreted as discrete events.
Game state data is continuously read and written to/from memory.

[ Summarized with AI ] <--

### 3.3 Software Interfaces
> Describe the connections between this product and other specific software components (name and version), including
> databases, operating systems, tools, libraries, and integrated commercial components. Identify the data items or
> messages coming into the system and going out and describe the purpose of each. Describe the services needed and
> the nature of communications. Refer to documents that describe detailed application programming interface protocols.
> Identify data that will be shared across software components. If the data sharing mechanism must be implemented in a
> specific way (for example, use of a global data area in a multitasking operating system), specify this as an
> implementation constraint.

* CMake
* Clion Nova
* MacOS
* Windows
* GLFW
* OpenGL
* Google Test? (Maybe)

### 3.4 Communications Interfaces
> Describe the requirements associated with any communications functions required by this product, including e-mail,
> web browser, network server communications protocols, electronic forms, and so on. Define any pertinent message
> formatting. Identify any communication standards that will be used, such as FTP or HTTP. Specify any communication
> security or encryption issues, data transfer rates, and synchronization mechanisms.

N/A

## System Features
> This template illustrates organizing the functional requirements for the product by system features, the major
> services provided by the product. You may prefer to organize this section by use case, mode of operation, user class,
> object class, functional hierarchy, or combinations of these, whatever makes the most logical sense for your product.


### 4.1 System Feature 1
> Don’t really say “System Feature 1.” State the feature name in just a few words.

##### 4.1.1   Description and Priority
> Provide a short description of the feature and indicate whether it is of High, Medium, or Low priority. You could
> also include specific priority component ratings, such as benefit, penalty, cost, and risk (each rated on a relative
> scale from a low of 1 to a high of 9).

##### 4.1.2   Stimulus/Response Sequences
> List the sequences of user actions and system responses that stimulate the behavior defined for this feature. These
> will correspond to the dialog elements associated with use cases.

##### 4.1.3   Functional Requirements
> Itemize the detailed functional requirements associated with this feature. These are the software capabilities that
> must be present in order for the user to carry out the services provided by the feature, or to execute the use case.
> Include how the product should respond to anticipated error conditions or invalid inputs. Requirements should be
> concise, complete, unambiguous, verifiable, and necessary. Use “TBD” as a placeholder to indicate when necessary
> information is not yet available.
>
> Each requirement should be uniquely identified with a sequence number or a meaningful tag of some kind.

### 4.2 System Feature 2 (and so on)


## Other Nonfunctional Requirements

### 5.1 Performance Requirements
> If there are performance requirements for the product under various circumstances, state them here and explain their
> rationale, to help the developers understand the intent and make suitable design choices. Specify the timing
> relationships for real time systems. Make such requirements as specific as possible. You may need to state performance
> requirements for individual functional requirements or features.

I'll need to find numbers somehow for metrics or standard tests to perform on the engine.

Obviously it needs to be very performant time wise and memory wise.


### 5.2 Safety Requirements
> Specify those requirements that are concerned with possible loss, damage, or harm that could result from the use of
> the product. Define any safeguards or actions that must be taken, as well as actions that must be prevented. Refer to
> any external policies or regulations that state safety issues that affect the product’s design or use. Define any
> safety certifications that must be satisfied.

Don't know how the game engine would do this?

N/A

### 5.3 Security Requirements
> Specify any requirements regarding security or privacy issues surrounding use of the product or protection of the
> data used or created by the product. Define any user identity authentication requirements. Refer to any external
> policies or regulations containing security issues that affect the product. Define any security or privacy
> certifications that must be satisfied.

At this time, the only security effort will be to not make any bugs and potential exploits via the code that I write purely
to build good habits in writing safe code. However, creating a DRM or securing the engine's state of integrity is out of
the scope of this project. May circle back to this at a later date cause its interesting but its too advanced for now.

### 5.4 Software Quality Attributes
> Specify any additional quality characteristics for the product that will be important to either the customers or the
> developers. Some to consider are: adaptability, availability, correctness, flexibility, interoperability,
> maintainability, portability, reliability, reusability, robustness, testability, and usability. Write these to be
> specific, quantitative, and verifiable when possible. At the least, clarify the relative preferences for various
> attributes, such as ease of use over ease of learning.



## Other Requirements
> Define any other requirements not covered elsewhere in the SRS. This might include database requirements,
> internationalization requirements, legal requirements, reuse objectives for the project, and so on. Add any new
> sections that are pertinent to the project.

N/A

---

### Appendix A: Glossary
> Define all the terms necessary to properly interpret the SRS, including acronyms and abbreviations. You may wish to
> build a separate glossary that spans multiple projects or the entire organization, and just include terms specific to
> a single project in each SRS.

N/A (so far?)

### Appendix B: Analysis Models
> Optionally, include any pertinent analysis models, such as data flow diagrams, class diagrams, state-transition
> diagrams, or entity-relationship diagrams.

Will Update as I go.

### Appendix C: To Be Determined List
> Collect a numbered list of the TBD (to be determined) references that remain in the SRS so they can be tracked to closure.



#### Template taken from (https://github.com/danielmavila/IEEE-SRS-Template/blob/master/README.md?plain=1)