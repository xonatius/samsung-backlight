# buildforkernels macro hint: when you build a new version or a new release
# that contains bugfixes or other improvements then you must disable the
# "buildforkernels newest" macro for just that build; immediately after
# queuing that build enable the macro again for subsequent builds; that way
# a new akmod package will only get build when a new one is actually needed
%define buildforkernels current

Name:		samsung-backlight-kmod
Version:	svn20101109
Release:	1%{?dist}
Summary:	Kernel module for Samsung laptops backlight

Group:		System Environment/Kernel
License:	GPLv2+
URL:		https://github.com/gregkh/samsung-backlight
# No direct links anymore. The sources are downloaded from the above page.
Source0:	%{name}-%{version}.tar.bz2
Source11:	%{name}tool-excludekernel-filterfile
Patch0:		%{name}-makefile.patch
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:	%{_bindir}/kmodtool

%{!?kernels:BuildRequires: buildsys-build-rpmfusion-kerneldevpkgs-%{?buildforkernels:%{buildforkernels}}%{!?buildforkernels:current}-%{_target_cpu} }

# kmodtool does its magic here
%{expand:%(kmodtool --target %{_target_cpu} --repo rpmfusion --kmodname %{name} --filterfile %{SOURCE11} %{?buildforkernels:--%{buildforkernels}} %{?kernels:--for-kernels "%{?kernels}"} 2>/dev/null) }

%description
This package contains the source code for Samsung Laptops Backlight driver

%prep
%{?kmodtool_check}

# print kmodtool output for debugging purposes:
kmodtool --target %{_target_cpu}  --repo rpmfusion --kmodname %{name} --filterfile %{SOURCE11} %{?buildforkernels:--%{buildforkernels}} %{?kernels:--for-kernels "%{?kernels}"} 2>/dev/null

%setup -q -c -T -a 0
pushd %{name}-%{version}
%patch0 -p1 -b .makefile-add-install
popd

# Fix permissions
for ext in c h; do
 find . -name "*.$ext" -exec chmod -x '{}' \;
done

for kernel_version in %{?kernel_versions} ; do
 cp -a %{name}-%{version} _kmod_build_${kernel_version%%___*}
done

%build
for kernel_version in %{?kernel_versions}; do
 make -C _kmod_build_${kernel_version%%___*} KERNELDIR="${kernel_version##*___}"
done

%install
rm -rf ${RPM_BUILD_ROOT}
for kernel_version in %{?kernel_versions}; do
 make -C _kmod_build_${kernel_version%%___*} KERNELPATH="${kernel_version##*___}" KERNELRELEASE="${kernel_version%%___*}" INST_DIR=${RPM_BUILD_ROOT}%{kmodinstdir_prefix}/${kernel_version%%___*}/%{kmodinstdir_postfix} install
done

chmod 0755 $RPM_BUILD_ROOT/%{kmodinstdir_prefix}/*/%{kmodinstdir_postfix}/*
%{?akmod_install}

%clean
rm -rf $RPM_BUILD_ROOT

%changelog
* Tue Nov  9 2010  Alexei Panov <elemc@atisserv.ru> - svn20101109-1
- Initial build. The patch add install instructions.
