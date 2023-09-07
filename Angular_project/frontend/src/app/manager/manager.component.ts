import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { UserService } from '../services/user.service';
import { User } from '../models/user';

@Component({
  selector: 'app-manager',
  templateUrl: './manager.component.html',
  styleUrls: ['./manager.component.css']
})
export class ManagerComponent implements OnInit {

  constructor(private router: Router, private userService: UserService) { }

  users: User[] = [];
  shownUsers: User[] = [];

  ngOnInit(): void {
    let type = localStorage.getItem("type");
    if(type != 'manager') this.router.navigate(['']);
    let username = localStorage.getItem("user");
    username = username == null ? "" : username; // check if username wasnt set
    if(username == "") this.logout;
    this.userService.getAllUsers().subscribe((data:User[])=>{
      this.users = data;
      this.shownUsers = data;
    })
  }

  logout() {
    localStorage.clear()
    this.router.navigate([''])
  }


  // Sorting properties
  private sortDirection = 1; // 1 for ascending, -1 for descending
  private currentSortKey = '';

  sortBy(key: string): void {
    this.sortDirection = 1;
    this.currentSortKey = key;
    this.shownUsers.sort((a, b) =>
      a[key].localeCompare(b[key]) * this.sortDirection
    );
  }

  reverseSortBy(key: string): void {
    this.sortDirection = -1;
    this.currentSortKey = key;
    this.shownUsers.sort((a, b) =>
      a[key].localeCompare(b[key]) * this.sortDirection
    );
  }

  // Search text object
  searchText = {
    firstname: '',
    lastname: '',
    type: '',
    username: ''
  };

  get filteredDoctors(): User[] {
    return this.users.filter(user =>
      (user.firstname.toLowerCase().includes(this.searchText.firstname.toLowerCase()) &&
      user.lastname.toLowerCase().includes(this.searchText.lastname.toLowerCase()) &&
      user.type.toLowerCase().includes(this.searchText.type.toLowerCase()) &&
      user.username.toLowerCase().includes(this.searchText.username.toLowerCase()))
    );
  }

  applyFilters(): void {
    this.shownUsers = this.filteredDoctors
  }

}
