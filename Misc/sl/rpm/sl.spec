Name:           sl
Version:        1.0
Release:        1%{?dist}
Summary:        New and improved version of sl
Source0:        %{name}-%{version}.tar.gz 
License:        MIT

BuildRequires: gcc
BuildRequires: make

%description
Builds sl.

%prep
mkdir -p %{_builddir}/bin
tar -xvzf %{_sourcedir}/%{name}-%{version}.tar.gz -C %{_builddir}

%build
cd %{_builddir}
make

%install
mkdir -p %{buildroot}/usr/local/bin
cp %{_builddir}/bin/sl %{buildroot}/usr/local/bin/. 

%files
/usr/local/bin/sl

%changelog
* Fri Jul  2 2021 tony
- 
