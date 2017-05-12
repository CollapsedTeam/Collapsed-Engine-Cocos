-- phpMyAdmin SQL Dump
-- version 4.5.2
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Czas generowania: 05 Lut 2016, 21:38
-- Wersja serwera: 5.5.46-0ubuntu0.14.04.2
-- Wersja PHP: 5.5.9-1ubuntu4.14

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Baza danych: `cllpsd`
--

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `accounts`
--

CREATE TABLE `accounts` (
  `id` int(10) UNSIGNED NOT NULL,
  `email` varchar(64) CHARACTER SET latin1 NOT NULL,
  `password` varchar(64) CHARACTER SET latin1 NOT NULL,
  `type` tinyint(3) UNSIGNED NOT NULL DEFAULT '1' COMMENT '1 normal, 2 super, 3 banned',
  `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Zrzut danych tabeli `accounts`
--

INSERT INTO `accounts` (`id`, `email`, `password`, `type`, `create_time`) VALUES
(1, 'admin@admin.com', '36b5b808ca93ce35fe773e55e0834688f49ce4e9', 2, '2016-02-05 20:22:23'),
(2, 'test@test.com', '588dc86d88d6a33dba2b1c7824df11e06277fbc9', 1, '2016-02-05 20:23:26');



--
-- Struktura tabeli dla tabeli `bannedips`
--
CREATE TABLE `bannedips` (
  `ip` varchar(16) COLLATE utf8_bin NOT NULL COMMENT 'ip of banned player',
  `reason` varchar(255) COLLATE utf8_bin NOT NULL,
  `start` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `end` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
-- --------------------------------------------------------
--
-- Struktura tabeli dla tabeli `characters`
--
CREATE TABLE `characters` (
  `id` int(10) UNSIGNED NOT NULL,
  `account` int(10) UNSIGNED NOT NULL,
  `name` varchar(32) COLLATE utf8_bin NOT NULL,
  `class` tinyint(3) UNSIGNED NOT NULL,
  `level` smallint(5) UNSIGNED NOT NULL DEFAULT '1',
  `sex` tinyint(1) UNSIGNED NOT NULL,
  `cash` int(10) UNSIGNED NOT NULL,
  `health` mediumint(10) UNSIGNED NOT NULL,
  `power` mediumint(10) UNSIGNED NOT NULL,
  `physique` smallint(5) UNSIGNED NOT NULL,
  `dexterity` smallint(5) UNSIGNED NOT NULL,
  `intelligence` smallint(5) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
-- --------------------------------------------------------
--
-- Struktura tabeli dla tabeli `classes`
--
CREATE TABLE `classes` (
  `id` int(10) UNSIGNED NOT NULL,
  `name` varchar(16) COLLATE utf8_bin NOT NULL,
  `cash` smallint(5) UNSIGNED NOT NULL,
  `health` tinyint(3) UNSIGNED NOT NULL,
  `power` tinyint(3) UNSIGNED NOT NULL,
  `physique` tinyint(3) UNSIGNED NOT NULL,
  `dexterity` tinyint(3) UNSIGNED NOT NULL,
  `intelligence` tinyint(3) UNSIGNED NOT NULL,
  `health_gain` tinyint(3) UNSIGNED NOT NULL,
  `power_gain` tinyint(3) UNSIGNED NOT NULL,
  `physique_gain` tinyint(3) UNSIGNED NOT NULL,
  `dexterity_gain` tinyint(3) UNSIGNED NOT NULL,
  `intelligence_gain` tinyint(3) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
--
-- Zrzut danych tabeli `classes`
--
INSERT INTO `classes` (`id`, `name`, `cash`, `health`, `power`, `physique`, `dexterity`, `intelligence`, `health_gain`, `power_gain`, `physique_gain`, `dexterity_gain`, `intelligence_gain`) VALUES
(1, 'Tester', 0, 45, 18, 6, 2, 3, 3, 1, 2, 1, 1),
(2, 'Xankoxy', 0, 88, 88, 10, 8, 8, 10, 6, 4, 4, 3);
-- --------------------------------------------------------
--
-- Struktura tabeli dla tabeli `dialogues`
--
CREATE TABLE `dialogues` (
  `id` int(11) NOT NULL,
  `dial_id` int(8) UNSIGNED NOT NULL,
  `char_id` int(10) UNSIGNED NOT NULL,
  `value` tinyint(3) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
-- --------------------------------------------------------
--
-- Struktura tabeli dla tabeli `guildmembers`
--
CREATE TABLE `guildmembers` (
  `char_id` int(10) UNSIGNED NOT NULL,
  `guild_id` int(10) UNSIGNED NOT NULL,
  `rank` tinyint(3) UNSIGNED NOT NULL,
  `note` varchar(32) COLLATE utf8_bin NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
-- --------------------------------------------------------
--
-- Struktura tabeli dla tabeli `guilds`
--
CREATE TABLE `guilds` (
  `id` int(10) UNSIGNED NOT NULL,
  `name` varchar(24) COLLATE utf8_bin NOT NULL,
  `tag` varchar(8) COLLATE utf8_bin NOT NULL,
  `description` text COLLATE utf8_bin NOT NULL,
  `rank0` varchar(16) COLLATE utf8_bin NOT NULL DEFAULT 'Recruit' COMMENT 'Name of recruit rank',
  `rank1` varchar(16) COLLATE utf8_bin NOT NULL DEFAULT 'Member' COMMENT 'Name of member rank',
  `rank2` varchar(16) COLLATE utf8_bin NOT NULL DEFAULT 'Manager' COMMENT 'Name of moderator rank',
  `rank3` varchar(16) COLLATE utf8_bin NOT NULL DEFAULT 'Owner' COMMENT 'Name of owner rank'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
--
-- Indeksy dla zrzutów tabel
--
--
-- Indexes for table `accounts`
--
ALTER TABLE `accounts`
  ADD PRIMARY KEY (`id`);
--
-- Indexes for table `bannedips`
--
ALTER TABLE `bannedips`
  ADD UNIQUE KEY `ip` (`ip`);
--
-- Indexes for table `characters`
--
ALTER TABLE `characters`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `name` (`name`);
--
-- Indexes for table `classes`
--
ALTER TABLE `classes`
  ADD PRIMARY KEY (`id`);
--
-- Indexes for table `dialogues`
--
ALTER TABLE `dialogues`
  ADD PRIMARY KEY (`id`);
--
-- Indexes for table `guildmembers`
--
ALTER TABLE `guildmembers`
  ADD PRIMARY KEY (`char_id`);
--
-- Indexes for table `guilds`
--
ALTER TABLE `guilds`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `tag` (`tag`);
--
-- AUTO_INCREMENT for dumped tables
--
--
-- AUTO_INCREMENT dla tabeli `accounts`
--
ALTER TABLE `accounts`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT dla tabeli `characters`
--
ALTER TABLE `characters`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT dla tabeli `classes`
--
ALTER TABLE `classes`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;
--
-- AUTO_INCREMENT dla tabeli `dialogues`
--
ALTER TABLE `dialogues`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT dla tabeli `guilds`
--
ALTER TABLE `guilds`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
