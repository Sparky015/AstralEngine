struct cpuinfo_mock_file filesystem[] = {
	{
		.path = "/proc/cpuinfo",
		.size = 280,
		.content = "Processor\t: ARMv7 Processor rev 2 (v7l)\n"
			   "BogoMIPS\t: 994.65\n"
			   "Features\t: swp half thumb fastmult vfp edsp thumbee neon vfpv3 \n"
			   "CPU implementer\t: 0x41\n"
			   "CPU architecture: 7\n"
			   "CPU variant\t: 0x2\n"
			   "CPU part\t: 0xc08\n"
			   "CPU revision\t: 2\n"
			   "\n"
			   "Hardware\t: herring\n"
			   "Revision\t: 0022\n"
			   "Serial\t\t: 363581cc0b5f00ec\n",
	},
	{
		.path = "/system/build.prop",
		.size = 2223,
		.content =
			"# begin build properties\n"
			"# autogenerated by buildinfo.sh\n"
			"ro.build.id=IMM76D\n"
			"ro.build.display.id=IMM76D\n"
			"ro.build.version.incremental=299849\n"
			"ro.build.version.sdk=15\n"
			"ro.build.version.codename=REL\n"
			"ro.build.version.release=4.0.4\n"
			"ro.build.date=Sun Mar 25 20:29:26 UTC 2012\n"
			"ro.build.date.utc=1332707366\n"
			"ro.build.type=user\n"
			"ro.build.user=android-build\n"
			"ro.build.host=vpbs7.mtv.corp.google.com\n"
			"ro.build.tags=release-keys\n"
			"ro.product.model=Nexus S 4G\n"
			"ro.product.brand=google\n"
			"ro.product.name=sojus\n"
			"ro.product.device=crespo4g\n"
			"ro.product.board=herring\n"
			"ro.product.cpu.abi=armeabi-v7a\n"
			"ro.product.cpu.abi2=armeabi\n"
			"ro.product.manufacturer=samsung\n"
			"ro.product.locale.language=en\n"
			"ro.product.locale.region=US\n"
			"ro.wifi.channels=\n"
			"ro.board.platform=s5pc110\n"
			"# ro.build.product is obsolete; use ro.product.device\n"
			"ro.build.product=crespo4g\n"
			"# Do not try to parse ro.build.description or .fingerprint\n"
			"ro.build.description=sojus-user 4.0.4 IMM76D 299849 release-keys\n"
			"ro.build.fingerprint=google/sojus/crespo4g:4.0.4/IMM76D/299849:user/release-keys\n"
			"ro.build.characteristics=nosdcard\n"
			"# end build properties\n"
			"#\n"
			"# system.prop for smdkc110\n"
			"#\n"
			"\n"
			"rild.libpath=/vendor/lib/libsec-ril.so\n"
			"rild.libargs=-d /dev/ttyS0\n"
			"ro.sf.lcd_density=240\n"
			"ril.ecclist=911,#911,*911\n"
			"\n"
			"#\n"
			"# ADDITIONAL_BUILD_PROPERTIES\n"
			"#\n"
			"ro.cdma.home.operator.numeric=310120\n"
			"ro.cdma.home.operator.alpha=Sprint\n"
			"ro.telephony.default_network=4\n"
			"ro.wimax.interface=uwbr0\n"
			"ro.opengles.version=131072\n"
			"wifi.interface=wlan0\n"
			"wifi.supplicant_scan_interval=15\n"
			"dalvik.vm.heapstartsize=5m\n"
			"dalvik.vm.heapgrowthlimit=48m\n"
			"dalvik.vm.heapsize=128m\n"
			"ro.config.ringtone=Sceptrum.ogg\n"
			"ro.config.notification_sound=Castor.ogg\n"
			"ro.com.android.dateformat=MM-dd-yyyy\n"
			"ro.com.android.dataroaming=false\n"
			"ro.url.legal=http://www.google.com/intl/%s/mobile/android/basic/phone-legal.html\n"
			"ro.url.legal.android_privacy=http://www.google.com/intl/%s/mobile/android/basic/privacy.html\n"
			"ro.com.google.clientidbase=android-google\n"
			"ro.config.alarm_alert=Alarm_Classic.ogg\n"
			"ro.com.android.wifi-watchlist=GoogleGuest\n"
			"ro.error.receiver.system.apps=com.google.android.feedback\n"
			"ro.setupwizard.enterprise_mode=1\n"
			"keyguard.no_require_sim=true\n"
			"drm.service.enabled=true\n"
			"dalvik.vm.dexopt-flags=m=y\n"
			"net.bt.name=Android\n"
			"dalvik.vm.stack-trace-file=/data/anr/traces.txt\n"
			"\n",
	},
	{
		.path = "/sys/devices/system/cpu/kernel_max",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/possible",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/present",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/online",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/offline",
		.size = 1,
		.content = "\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpuidle/current_driver",
		.size = 9,
		.content = "s5p_idle\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpuidle/current_governor_ro",
		.size = 5,
		.content = "menu\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/affected_cpus",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "1000000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "100000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_transition_latency",
		.size = 6,
		.content = "40000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/related_cpus",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_governors",
		.size = 67,
		.content = "conservative ondemand userspace powersave interactive performance \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq",
		.size = 8,
		.content = "1000000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_driver",
		.size = 8,
		.content = "s5pv210\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor",
		.size = 9,
		.content = "ondemand\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq",
		.size = 8,
		.content = "1000000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq",
		.size = 7,
		.content = "200000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/stats/time_in_state",
		.size = 56,
		.content = "1000000 2511\n"
			   "800000 211\n"
			   "400000 223\n"
			   "200000 233\n"
			   "100000 28\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/stats/total_trans",
		.size = 3,
		.content = "88\n",
	},
	{NULL},
};

#ifdef __ANDROID__
struct cpuinfo_mock_property properties[] = {
	{
		.key = "dalvik.vm.dexopt-flags",
		.value = "m=y",
	},
	{
		.key = "dalvik.vm.heapgrowthlimit",
		.value = "48m",
	},
	{
		.key = "dalvik.vm.heapsize",
		.value = "128m",
	},
	{
		.key = "dalvik.vm.heapstartsize",
		.value = "5m",
	},
	{
		.key = "dalvik.vm.stack-trace-file",
		.value = "/data/anr/traces.txt",
	},
	{
		.key = "dev.bootcomplete",
		.value = "1",
	},
	{
		.key = "drm.service.enabled",
		.value = "true",
	},
	{
		.key = "gsm.current.phone-type",
		.value = "2",
	},
	{
		.key = "gsm.network.type",
		.value = "EvDo-rev.A:8",
	},
	{
		.key = "gsm.nitz.time",
		.value = "1505256706443",
	},
	{
		.key = "gsm.operator.alpha",
		.value = "Sprint",
	},
	{
		.key = "gsm.operator.iso-country",
		.value = "us",
	},
	{
		.key = "gsm.operator.isroaming",
		.value = "false",
	},
	{
		.key = "gsm.operator.numeric",
		.value = "310120",
	},
	{
		.key = "gsm.sim.operator.alpha",
		.value = "Sprint",
	},
	{
		.key = "gsm.sim.operator.iso-country",
		.value = "us",
	},
	{
		.key = "gsm.sim.operator.numeric",
		.value = "310120",
	},
	{
		.key = "gsm.sim.state",
		.value = "READY",
	},
	{
		.key = "gsm.version.baseband",
		.value = "D720SPRLC1 ",
	},
	{
		.key = "gsm.version.ril-impl",
		.value = "RIL 980955",
	},
	{
		.key = "init.svc.adbd",
		.value = "running",
	},
	{
		.key = "init.svc.bluetoothd",
		.value = "running",
	},
	{
		.key = "init.svc.bootanim",
		.value = "stopped",
	},
	{
		.key = "init.svc.dbus",
		.value = "running",
	},
	{
		.key = "init.svc.debuggerd",
		.value = "running",
	},
	{
		.key = "init.svc.drm",
		.value = "running",
	},
	{
		.key = "init.svc.flash_recovery",
		.value = "stopped",
	},
	{
		.key = "init.svc.gpsd",
		.value = "running",
	},
	{
		.key = "init.svc.hciattach",
		.value = "running",
	},
	{
		.key = "init.svc.installd",
		.value = "running",
	},
	{
		.key = "init.svc.keystore",
		.value = "running",
	},
	{
		.key = "init.svc.media",
		.value = "running",
	},
	{
		.key = "init.svc.netd",
		.value = "running",
	},
	{
		.key = "init.svc.pvrsrvinit",
		.value = "stopped",
	},
	{
		.key = "init.svc.ril-daemon",
		.value = "running",
	},
	{
		.key = "init.svc.servicemanager",
		.value = "running",
	},
	{
		.key = "init.svc.setup_fs",
		.value = "stopped",
	},
	{
		.key = "init.svc.surfaceflinger",
		.value = "running",
	},
	{
		.key = "init.svc.vold",
		.value = "running",
	},
	{
		.key = "init.svc.zygote",
		.value = "running",
	},
	{
		.key = "keyguard.no_require_sim",
		.value = "true",
	},
	{
		.key = "net.bt.name",
		.value = "Android",
	},
	{
		.key = "net.cdma.configurable.mode",
		.value = "-1",
	},
	{
		.key = "net.change",
		.value = "net.cdma.configurable.mode",
	},
	{
		.key = "net.hostname",
		.value = "android-2b040bd45a1b2a33",
	},
	{
		.key = "net.qtaguid_enabled",
		.value = "1",
	},
	{
		.key = "net.tcp.buffersize.default",
		.value = "4096,87380,110208,4096,16384,110208",
	},
	{
		.key = "net.tcp.buffersize.edge",
		.value = "4093,26280,35040,4096,16384,35040",
	},
	{
		.key = "net.tcp.buffersize.gprs",
		.value = "4092,8760,11680,4096,8760,11680",
	},
	{
		.key = "net.tcp.buffersize.hspa",
		.value = "4094,87380,262144,4096,16384,262144",
	},
	{
		.key = "net.tcp.buffersize.lte",
		.value = "524288,1048576,2097152,262144,524288,1048576",
	},
	{
		.key = "net.tcp.buffersize.umts",
		.value = "4094,87380,110208,4096,16384,110208",
	},
	{
		.key = "net.tcp.buffersize.wifi",
		.value = "524288,1048576,2097152,262144,524288,1048576",
	},
	{
		.key = "net.tcp.buffersize.wimax",
		.value = "4096,524288,1048576,4096,16384,110208",
	},
	{
		.key = "persist.sys.country",
		.value = "US",
	},
	{
		.key = "persist.sys.language",
		.value = "en",
	},
	{
		.key = "persist.sys.localevar",
		.value = "",
	},
	{
		.key = "persist.sys.profiler_ms",
		.value = "0",
	},
	{
		.key = "persist.sys.timezone",
		.value = "America/Los_Angeles",
	},
	{
		.key = "persist.sys.usb.config",
		.value = "mass_storage,adb",
	},
	{
		.key = "ril.MSL",
		.value = "279803",
	},
	{
		.key = "ril.OTKSL",
		.value = "944368",
	},
	{
		.key = "ril.bt_macaddr",
		.value = "001247012345",
	},
	{
		.key = "ril.ecclist",
		.value = "911,#911,*911",
	},
	{
		.key = "ril.hw_ver",
		.value = "H:D720.06 S.Q  ",
	},
	{
		.key = "ril.model_id",
		.value = "",
	},
	{
		.key = "ril.otksl_flag",
		.value = "0",
	},
	{
		.key = "ril.prl_num",
		.value = "1",
	},
	{
		.key = "ril.prl_ver_1",
		.value = "P:61083        ",
	},
	{
		.key = "ril.product_code",
		.value = "",
	},
	{
		.key = "ril.rfcal_date",
		.value = "0000.00.00",
	},
	{
		.key = "ril.sw_ver",
		.value = "D720SPRLC1 ",
	},
	{
		.key = "rild.libargs",
		.value = "-d /dev/ttyS0",
	},
	{
		.key = "rild.libpath",
		.value = "/vendor/lib/libsec-ril.so",
	},
	{
		.key = "ro.allow.mock.location",
		.value = "0",
	},
	{
		.key = "ro.baseband",
		.value = "D720SPRLC1",
	},
	{
		.key = "ro.board.platform",
		.value = "s5pc110",
	},
	{
		.key = "ro.bootloader",
		.value = "D720SPRKE1",
	},
	{
		.key = "ro.bootmode",
		.value = "unknown",
	},
	{
		.key = "ro.bt.bdaddr_path",
		.value = "/efs/bluetooth/bt_addr",
	},
	{
		.key = "ro.build.characteristics",
		.value = "nosdcard",
	},
	{
		.key = "ro.build.date.utc",
		.value = "1332707366",
	},
	{
		.key = "ro.build.date",
		.value = "Sun Mar 25 20:29:26 UTC 2012",
	},
	{
		.key = "ro.build.description",
		.value = "sojus-user 4.0.4 IMM76D 299849 release-keys",
	},
	{
		.key = "ro.build.display.id",
		.value = "IMM76D",
	},
	{
		.key = "ro.build.fingerprint",
		.value = "google/sojus/crespo4g:4.0.4/IMM76D/299849:user/release-keys",
	},
	{
		.key = "ro.build.host",
		.value = "vpbs7.mtv.corp.google.com",
	},
	{
		.key = "ro.build.id",
		.value = "IMM76D",
	},
	{
		.key = "ro.build.product",
		.value = "crespo4g",
	},
	{
		.key = "ro.build.tags",
		.value = "release-keys",
	},
	{
		.key = "ro.build.type",
		.value = "user",
	},
	{
		.key = "ro.build.user",
		.value = "android-build",
	},
	{
		.key = "ro.build.version.codename",
		.value = "REL",
	},
	{
		.key = "ro.build.version.incremental",
		.value = "299849",
	},
	{
		.key = "ro.build.version.release",
		.value = "4.0.4",
	},
	{
		.key = "ro.build.version.sdk",
		.value = "15",
	},
	{
		.key = "ro.carrier",
		.value = "SPR",
	},
	{
		.key = "ro.cdma.home.operator.alpha",
		.value = "Sprint",
	},
	{
		.key = "ro.cdma.home.operator.numeric",
		.value = "310120",
	},
	{
		.key = "ro.com.android.dataroaming",
		.value = "false",
	},
	{
		.key = "ro.com.android.dateformat",
		.value = "MM-dd-yyyy",
	},
	{
		.key = "ro.com.android.wifi-watchlist",
		.value = "GoogleGuest",
	},
	{
		.key = "ro.com.google.clientidbase",
		.value = "android-google",
	},
	{
		.key = "ro.config.alarm_alert",
		.value = "Alarm_Classic.ogg",
	},
	{
		.key = "ro.config.notification_sound",
		.value = "Castor.ogg",
	},
	{
		.key = "ro.config.ringtone",
		.value = "Sceptrum.ogg",
	},
	{
		.key = "ro.crypto.fs_flags",
		.value = "0x00000406",
	},
	{
		.key = "ro.crypto.fs_mnt_point",
		.value = "/data",
	},
	{
		.key = "ro.crypto.fs_options",
		.value = "nomblk_io_submit",
	},
	{
		.key = "ro.crypto.fs_real_blkdev",
		.value = "/dev/block/platform/s3c-sdhci.0/by-name/userdata",
	},
	{
		.key = "ro.crypto.fs_type",
		.value = "ext4",
	},
	{
		.key = "ro.crypto.keyfile.userdata",
		.value = "/efs/userdata_footer",
	},
	{
		.key = "ro.crypto.state",
		.value = "unencrypted",
	},
	{
		.key = "ro.debuggable",
		.value = "0",
	},
	{
		.key = "ro.error.receiver.system.apps",
		.value = "com.google.android.feedback",
	},
	{
		.key = "ro.factorytest",
		.value = "0",
	},
	{
		.key = "ro.hardware",
		.value = "herring",
	},
	{
		.key = "ro.nfc.port",
		.value = "I2C",
	},
	{
		.key = "ro.opengles.version",
		.value = "131072",
	},
	{
		.key = "ro.product.board",
		.value = "herring",
	},
	{
		.key = "ro.product.brand",
		.value = "google",
	},
	{
		.key = "ro.product.cpu.abi2",
		.value = "armeabi",
	},
	{
		.key = "ro.product.cpu.abi",
		.value = "armeabi-v7a",
	},
	{
		.key = "ro.product.device",
		.value = "crespo4g",
	},
	{
		.key = "ro.product.locale.language",
		.value = "en",
	},
	{
		.key = "ro.product.locale.region",
		.value = "US",
	},
	{
		.key = "ro.product.manufacturer",
		.value = "samsung",
	},
	{
		.key = "ro.product.model",
		.value = "Nexus S 4G",
	},
	{
		.key = "ro.product.name",
		.value = "sojus",
	},
	{
		.key = "ro.radio.noril",
		.value = "yes",
	},
	{
		.key = "ro.revision",
		.value = "34",
	},
	{
		.key = "ro.ril.MEID",
		.value = "A000002A635782",
	},
	{
		.key = "ro.runtime.firstboot",
		.value = "1505256709934",
	},
	{
		.key = "ro.secure",
		.value = "1",
	},
	{
		.key = "ro.serialno",
		.value = "363581CC0B5F00EC",
	},
	{
		.key = "ro.setupwizard.enterprise_mode",
		.value = "1",
	},
	{
		.key = "ro.sf.lcd_density",
		.value = "240",
	},
	{
		.key = "ro.telephony.call_ring.multiple",
		.value = "0",
	},
	{
		.key = "ro.telephony.default_network",
		.value = "4",
	},
	{
		.key = "ro.url.legal.android_privacy",
		.value = "http://www.google.com/intl/%s/mobile/android/basic/privacy.html",
	},
	{
		.key = "ro.url.legal",
		.value = "http://www.google.com/intl/%s/mobile/android/basic/phone-legal.html",
	},
	{
		.key = "ro.wifi.channels",
		.value = "",
	},
	{
		.key = "ro.wimax.interface",
		.value = "uwbr0",
	},
	{
		.key = "status.battery.level",
		.value = "5",
	},
	{
		.key = "status.battery.level_raw",
		.value = "50",
	},
	{
		.key = "status.battery.level_scale",
		.value = "9",
	},
	{
		.key = "status.battery.state",
		.value = "Slow",
	},
	{
		.key = "sys.boot_completed",
		.value = "1",
	},
	{
		.key = "sys.usb.config",
		.value = "mass_storage,adb",
	},
	{
		.key = "sys.usb.state",
		.value = "mass_storage,adb",
	},
	{
		.key = "system_init.startsurfaceflinger",
		.value = "0",
	},
	{
		.key = "vold.post_fs_data_done",
		.value = "1",
	},
	{
		.key = "wifi.interface",
		.value = "wlan0",
	},
	{
		.key = "wifi.supplicant_scan_interval",
		.value = "15",
	},
	{NULL},
};
#endif /* __ANDROID__ */
